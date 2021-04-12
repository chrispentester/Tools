#PowerCLI 6.3 script that gets user data from csv file then provisions VMs based off that information
#Version 1.5
#Created by: Chris Sikes & Bruce MacDougall

#########################################################################################################
#													#
#	Sample .csv or .txt file format for the Import-Csv cmdlet.					#
#	The first line of the .csv has the key values and all lines after represent one Virtual machine.#
#													#
#	name,type,template,cluster,datastore,ip,subnet,gateway,vlan					#
#													#		
#########################################################################################################

#vCenter login information
$vcenterip = "<vcenter IP>"
$vcusername = "<admin>@vsphere.local"
$vcpassword = "<password>"
$myfolder = "Hacking Training"

#Get start time
$startDTM = (get-date)

#Connects to vCenter Server
connect-viserver $vcenterip -user $vcusername -password $vcpassword

#Remove any leftover -nonpersistant oscustomizationspecs from previous testing
#get-oscustomizationspec -type nonpersistent | remove-oscustomizationspec -confirm:$false

#Define constant variables to be used for VM creation
$csvfile = ".\hacklab.csv"

$resourcepool = "Hack_Lab"

#$custWinSpec = "WinSpec"
#$custLinSpec = "LinSpec"

#Create temporary customizationspecs for each VM type
#$specWinClone = New-OSCustomizationSpec -Name myWinSpec -Spec $custWinSpec -Type NonPersistent
#$specClone = New-OSCustomizationSpec -Name myLinSpec -Spec $custLinSpec -Type NonPersistent

#Import CSV file into a list
$systems = Import-CSV $csvfile -encoding UTF8

#Create a blank hashtable to capture newly created VMs
$hashTable = @{}
$hashTable1 = @{}

foreach($item in $systems){
	$hashTable1[$item.name] = $item.portgroup,$item.mac,$item.vmhostname
}

#Write-host "Press any key to continue..."
#$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

#Create VM and Apply Cust Spec to each VM
Foreach($system in $systems){

	#set dyanmic variables
	$name = $system.name
	$datastore = $system.datastore
	$ip1 = $system.ip1
	$subnet1 = $system.subnet1
	$gateway1 = $system.gateway1
	$cluster = $system.cluster
	#$vmhost = Get-VMhost -name $system.vmhostname
	$vmhost = $system.vmhostname
	$type = $system.type
	$macaddress = $system.mac
	$template = $system.type

	#Create VMs and stores them in a hashtable
	$Hashtable[(New-VM -Name $name -Template $template -Location $myfolder -Datastore $datastore -resourcepool $resourcepool -vmhost $vmhost -Notes $macaddress -erroraction silentlycontinue -RunAsync).Id] = $system
}

$runningTasks1 = $HashTable.count

while($runningTasks1 -gt 0){
	Get-Task | % {
		if($HashTable.ContainsKey($_.Id) -and $_.State -eq "Success"){
			$vmHashName = $HashTable[$_.Id].name
			$portgroupname = $hashTable1.$vmHashName[0]
			$portgroupobj = get-vmhost -vm $vmHashName | get-virtualportgroup -name $portgroupname
			$macaddressid = $hashTable1.$vmHashName[1]
			Get-VM $vmHashName | Get-NetworkAdapter | Set-NetworkAdapter -portgroup $portgroupobj -confirm:$false
			Get-VM $vmHashName | Get-NetworkAdapter | Set-NetworkAdapter -MacAddress $macaddressid -StartConnected:$true -Confirm:$false
			Start-VM $vmHashName -runasync
			$HashTable.Remove($_.Id)
      			$runningTasks1--
    		}
  	}
	Write-host "Waiting for $runningTasks1 VMs to be powered on"
	Start-Sleep -Seconds 15
}

#disconnect-viserver $vcenterip -confirm:$false

#Get end time
$endDTM = (get-date)

#Calculate elapsed time
"Elapsed Time: $(($endDTM-$startDTM).totalseconds) seconds"