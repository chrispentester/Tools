#! /usr/bin/python
# Written by Christopher Sikes

import os
from gps import *
from time import *
import time
import threading
import sys
import subprocess


interface = "wlan0"

f = open('myfile', 'w')


gpsd = None #setting the global variable

os.system('clear') #clear the terminal (optional)


def get_name(cell):
    return matching_line(cell,"ESSID:")[1:-1]

def get_quality(cell):
    quality = matching_line(cell,"Quality=").split()[0].split('/')
    return str(int(round(float(quality[0]) / float(quality[1]) * 100))).rjust(3) + " %"

def get_channel(cell):
    return matching_line(cell,"Channel:")

def get_signal_level(cell):
    # Signal level is on same line as Quality data so a bit of ugly
    # hacking needed...
    return matching_line(cell,"Quality=").split("Signal level=")[1]


def get_encryption(cell):
    enc=""
    if matching_line(cell,"Encryption key:") == "off":
        enc="Open"
    else:
        for line in cell:
            matching = match(line,"IE:")
            if matching!=None:
                wpa=match(matching,"WPA Version ")
                if wpa!=None:
                    enc="WPA v."+wpa
        if enc=="":
            enc="WEP"
    return enc

def get_address(cell):
    return matching_line(cell,"Address: ")

class GpsPoller(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)
    global gpsd #bring it in scope
    gpsd = gps(mode=WATCH_ENABLE) #starting the stream of info
    self.current_value = None
    self.running = True #setting the thread running to true

  def run(self):
    global gpsd
    while gpsp.running:
      gpsd.next() #this will continue to loop and grab EACH set of gpsd info to clear th$


rules={"Name":get_name,
       "Quality":get_quality,
       "Channel":get_channel,
       "Encryption":get_encryption,
       "Address":get_address,
       "Signal":get_signal_level
       }

def sort_cells(cells):
    sortby = "Quality"
    reverse = True
    cells.sort(None, lambda el:el[sortby], reverse)

columns=["Name","Address","Quality","Signal", "Channel","Encryption"]

def matching_line(lines, keyword):
    """Returns the first matching line in a list of lines. See match()"""
    for line in lines:
        matching=match(line,keyword)
        if matching!=None:
            return matching
    return None

def match(line,keyword):
    """If the first part of line (modulo blanks) matches keyword,
    returns the end of that line. Otherwise returns None"""
    line=line.lstrip()
    length=len(keyword)
    if line[:length] == keyword:
        return line[length:]
    else:
        return None

def parse_cell(cell):
    """Applies the rules to the bunch of text describing a cell and returns the
    corresponding dictionary"""
    parsed_cell={}
    for key in rules:
        rule=rules[key]
        parsed_cell.update({key:rule(cell)})
    return parsed_cell

def print_table(table):
    widths=map(max,map(lambda l:map(len,l),zip(*table))) #functional magic

    justified_table = []
    for line in table:
        justified_line=[]
        for i,el in enumerate(line):
            justified_line.append(el.ljust(widths[i]+2))
        justified_table.append(justified_line)

    for line in justified_table:
        for el in line:
            print el,
        print


def print_cells(cells):
    table=[columns]
    for cell in cells:
        cell_properties=[]
        for column in columns:
            cell_properties.append(cell[column])
        table.append(cell_properties)
    print_table(table)





if __name__ == '__main__':
  gpsp = GpsPoller() # create the thread
  try:
    gpsp.start() # start it up
    while True:
      #It may take a second or two to get good data
      #print gpsd.fix.latitude,', ',gpsd.fix.longitude,'  Time: ',gpsd.utc

      os.system('clear')

      print
      print ' GPS reading'
      print '----------------------------------------'
      print 'latitude    ' , gpsd.fix.latitude
      print 'longitude   ' , gpsd.fix.longitude
      print 'time utc    ' , gpsd.utc


      if gpsd.utc != '':
          value = (gpsd.fix.latitude)
          if value != 'nan':
              cells=[[]]
              parsed_cells=[]

              proc = subprocess.Popen(["sudo", "iwlist", interface, "scan"],stdout=subprocess.PIPE, universal_newlines=True)
              out, err = proc.communicate()
              for line in out.split("\n"):
                  cell_line = match(line,"Cell ")
                  if cell_line != None:
                      cells.append([])
                      line = cell_line[-27:]
                  cells[-1].append(line.rstrip())

              cells=cells[1:]

              for cell in cells:
                parsed_cells.append(parse_cell(cell))
                f.write("latitude = ")
                value = (gpsd.fix.latitude)
                s = str(value)
                f.write(s)
                f.write(", longitude = ")
                value = (gpsd.fix.longitude)
                s = str(value)
                f.write(s)
                f.write(", time utc = ")
                value = (gpsd.utc)
                s = str(value)
                f.write(s)
                f.write(", ")
                value = (parse_cell(cell))
                s = str(value)
                f.write(s)
                f.write("\n")
              sort_cells(parsed_cells)
              print_cells(parsed_cells)







      time.sleep(5) #set to whatever

  except (KeyboardInterrupt, SystemExit): #when you press ctrl+c
    print "\nKilling Thread..."
    gpsp.running = False
    gpsp.join() # wait for the thread to finish what it's doing
  print "Done.\nExiting."
