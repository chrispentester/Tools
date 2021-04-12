/////////////////////////////////////////////////////////////////////////////
// Name:        MSXML_Wrapper.cpp
// Version:     1.0
// Author:		Chris Sikes
// Purpose:     Uses simple MSXML library Wrapper class to modify DOM XML files
// Notes:		Fixes multiple compile issues with XMLParser to work with current Visual Studio 2015 & Win8 build machine
/////////////////////////////////////////////////////////////////////////////
// MSXML_Wrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>
#include "XMLParser.h"
#include <iostream>
#include <string>
#include <conio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#undef max

// The one and only application object

CWinApp theApp;

using namespace MSXML2;
using namespace std;




int main()
{

	XML_PARSER parser;

	string name;
	string xmlname;
	string xmlname2;
	string xmlname3;
	string counts = "";
	int countz = 0;
	int truth = 0;
	int truth2 = 0;
	int counter = 0;
	wstring xmlnameL;
	wstring xmlnameL2;
	wstring xmlnameL3;
	wstring namew;
	LPCTSTR xmlnameLPC;
	LPCTSTR xmlnameLPC2;
	LPCTSTR xmlnameLPC3;

	cout << "---------------------------------" << endl;
	cout << "Welcome to the MSXML Wrapper Tool" << endl;
	cout << "Select an option" << endl;
	cout << "1. Load XML file and display contents" << endl;
	cout << "2. Load XML file and save file" << endl;
	cout << "3. Parse XML File Menu: Traverse, modify, and replace" << endl;
	cout << "4. Create new XML file and save" << endl;
	
	
	cout << "> ";
	cin >> counts;

	while (1)
	{
		if (((counts[0] == '1') || (counts[0] == '2') || (counts[0] == '3') || (counts[0] == '4')) && (counts.length()==1))
		{
			countz = atoi(counts.c_str());
			break;
		}
		else
		{
			cout << "You have entered a wrong input" << endl;
			cout << "Select an option" << endl;
			cout << "1. Load XML file and display contents" << endl;
			cout << "2. Load XML file and save file" << endl;
			cout << "3. Parse XML File Menu: Traverse, modify, and replace" << endl;
			cout << "4. Create new XML file and save" << endl;
			cout << "> ";
			cin >> counts;
		}

	}


	cout << "You selected option " << countz << endl;

	if (countz == 1)   // needs input validation
	{
		while (1)
		{
			cout << "What's the path/filename.xml of the file you want to display contents of?" << endl;
			cout << "> ";
			cin >> xmlname;
			xmlnameL = wstring(xmlname.begin(), xmlname.end());
			if (parser.Load_XML_Document(xmlnameL.c_str()) == false)
			{
				cout << "File not found! Try again." << endl;
			}
			else
			{
				break;
			}
		}

		parser.Display_Contents();
	}

	if (countz == 2)  // needs input validation
	{
		while (1)
		{
			cout << "What's the path/filename.xml of the file of interest?" << endl;
			cout << "> ";
			cin >> xmlname;
			xmlnameL = wstring(xmlname.begin(), xmlname.end());

			if (parser.Load_XML_Document(xmlnameL.c_str()) == false)
			{
				cout << "File not found! Try again." << endl;
			}
			else
			{
				break;
			}
		}
			cout << "What's the path/filename.xml name you want to save the file as?" << endl;
			cout << "> ";
			cin >> xmlname2;
			xmlnameL2 = wstring(xmlname2.begin(), xmlname2.end());
			parser.Save_XML_Document(xmlnameL2.c_str());

	}

	if (countz == 3)
	{
		int n;
		int option;
		int option2;
		string countss = "";
		string filename;
		cout << "What's the path/filename.xml of the file you want to parse through and replace nodes?" << endl;
		cout << "> ";
		cin >> xmlname;
		filename = xmlname;
		xmlnameL = wstring(xmlname.begin(), xmlname.end());
		parser.Load_XML_Document(xmlnameL.c_str());
		



		while (1)
		{
			cout << "Please choose an option while parsing your file" << endl;
			cout << "1: Display Contents" << endl;
			cout << "2: Display Current Node and Traverse" << endl;
			cout << "3: Display Current Node Attributes" << endl;
			cout << "4: Change Current Node Attributes" << endl;
			cout << "5: Save file" << endl;
			cout << "6: Exit" << endl;
			cin >> countss;

			if (((countss[0] == '1') || (countss[0] == '2') || (countss[0] == '3') || (countss[0] == '4') || (countss[0] == '5') || (countss[0] == '6')) && (countss.length() == 1))
			{
				option = atoi(countss.c_str());




				if (option == 1)
				{
					parser.Display_Contents();
				}
				else if (option == 2)
				{
					//
					wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;

					while (1)
					{
						cout << "Please select an option:" << endl;
						cout << "1: Display current node" << endl;
						cout << "2: Go to Root Node" << endl;
						cout << "3: Go to Child Node" << endl;
						cout << "4: Go to Parent Node" << endl;
						cout << "5: Traverse Forwards" << endl;
						cout << "6: Traverse Backwards" << endl;
						cout << "7: Back to Main Parsing Menu" << endl;
						cin >> option2;

						if (option2 == 1)
						{
							if (parser.Is_Root() == 1)
							{
								cout << "You are at the Root node" << endl << endl;
								//getch();
							}
							else
							{
								wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;
							//	getch();
							}

						}
						else if (option2 == 2)
						{
							cout << "Traversing to Root." << endl;
							parser.Go_to_Root();
							wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;

						}
						else if (option2 == 3)
						{
							cout << "Traversing to first child of current node." << endl;
							parser.Go_to_Child();
							wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;

						}
						else if (option2 == 4)
						{
							cout << "Traversing to parent node of current node." << endl;
							parser.Go_to_Parent();
							wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;

						}
						else if (option2 == 5)
						{
							if (parser.Go_Forward())
							{
								cout << "Traversing forward to next node on the same level." << endl;
								wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;

							}
							else
							{
								cout << "You are at the last node" << endl;
								wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;

							}
							
						}
						else if (option2 == 6)
						{
							if (parser.Go_Backward())
							{
								cout << "Traversing backwards to previous node on same level." << endl;
								wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;
							}
							else
							{
								cout << "You are at the last node" << endl;
								wcout << "You are at the " << (const wchar_t*)parser.Get_CurrentName() << " node" << endl;
							}
													


						}
						else if (option2 ==7)
						{
							break;
						}
						else
						{
							cout << "That is not a valid input! Try again!" << endl;
						}


					}

				}
				else if (option == 3)  // doesn't work. get_attr_name is the address of the value but isn't actually deleted
				{
					if (parser.Is_Root() == 1)
					{
						cout << "You are at the Root node, please change to a child node." << endl << endl;
						//getch();
					}
					else
					{
						cout << "There are " << parser.Get_Attribute_Count() << " Attributes in this node." << endl;
						counter = parser.Get_Attribute_Count();
						n = 0;

						while (0 < counter) {
							wcout << "Attribute Name " << n + 1 << ": " << (const wchar_t*)parser.Get_Attribute_Name(n) << endl;
							wcout << "Value " << ": " << (const wchar_t*)parser.Get_Attribute_Value(n) << endl;
							counter--;
							n++;
						}

					}
					
				}


				else if (option == 4)
				{
					truth = 1;
					if (parser.Is_Root() == 1)
					{
						cout << "You are at the Root node, please change to a child node." << endl << endl;
						cout << "Do you want to continue?" << endl;
						cout << "Enter 1 for YES and 0 for NO." << endl;
						cin >> truth;
					}
					if(truth == 1)
					{
						namew = (const wchar_t*)parser.Get_CurrentName();
						while (1)
						{
							cout << "Please select an option:" << endl;
							cout << "1: Add Attribute" << endl;
							cout << "2: Remove Attribute" << endl;
							cout << "3: Back to Main Parsing Menu" << endl;

							cin >> option2;

							if (option2 == 1)
							{
								cout << "Add an Attribute!" << endl;
								wcout << "What's the " << namew << " attribute name?" << endl;
								cout << "(ex. Id)" << endl;
								cout << "> ";
								cin >> xmlname;
								xmlnameL = wstring(xmlname.begin(), xmlname.end());
								xmlnameLPC = xmlnameL.c_str();

								wcout << "What's the " << namew << " attribute value?" << endl;
								cout << "(ex. rId1)" << endl;
								cout << "> ";
								cin >> xmlname2;
								xmlnameL2 = wstring(xmlname2.begin(), xmlname2.end());
								xmlnameLPC2 = xmlnameL2.c_str();
								parser.Set_Attribute(xmlnameLPC, xmlnameLPC2);
							}
							else if (option2 == 2)
							{
								cout << "Remove an Attribute!" << endl;
								wcout << "What's the " << namew << " attribute name?" << endl;
								cout << "(ex. Id)" << endl;
								cout << "> ";
								cin >> xmlname;
								xmlnameL = wstring(xmlname.begin(), xmlname.end());
								xmlnameLPC = xmlnameL.c_str();
								parser.Remove_Attribute(xmlnameLPC);
								parser.Refresh_Contents();
							}
							else if (option2 == 3)
							{
								break;
							}
							else
							{
								cout << "That is not a valid input! Try again!" << endl;
							}
						}

					}
					



				}
				else if (option == 5)
				{
					// Save xml file
					cout << "Overwriting file!" << endl;
					xmlnameL = wstring(filename.begin(), filename.end());
					parser.Save_XML_Document(xmlnameL.c_str());
				}
				else if (option == 6)
				{
					cout << "Good bye!" << endl;
					break;
				}
				else
				{
					cout << "That is not a valid input! Try again!" << endl;
				}
			}
			else
			{
				cout << "You have entered a wrong input." << endl;
			}

		}

	}


	if (countz == 4)   // Not Complete Yet, need input validation and fixing up minor issues
	{
	
		// Reset content of xml document
		parser.Reset_XML_Document();

		// create header, set encoding property
		cout << "Set the Header:" << endl;
		cout << "What header? (ex. xml)" << endl;
		cout << "> ";
		cin >> xmlname;
		xmlnameL = wstring(xmlname.begin(), xmlname.end());
		cout << "What name? (ex. encoding)" << endl;
		cout << "> ";
		cin >> xmlname2;
		xmlnameL2 = wstring(xmlname2.begin(), xmlname2.end());
		cout << "What value? (ex. UTF-8)" << endl;
		cout << "> ";
		cin >> xmlname3;
		xmlnameL3 = wstring(xmlname3.begin(), xmlname3.end());
		xmlnameLPC = xmlnameL.c_str();
		xmlnameLPC2 = xmlnameL2.c_str();
		xmlnameLPC3 = xmlnameL3.c_str();

		parser.Set_Header(xmlnameLPC,xmlnameLPC2,xmlnameLPC3);

		// create root node
		cout << "Set the root node:" << endl;
		cout << "What root node? (ex. Relationships)" << endl;
		cout << "> ";
		cin >> xmlname;
		xmlnameL = wstring(xmlname.begin(), xmlname.end());
		xmlnameLPC = xmlnameL.c_str();
		parser.Add_FirstChildNode(xmlnameLPC);

		cout << "What root attribute name? (ex. xmlns)" << endl;
		cout << "> ";
		cin >> xmlname;
		xmlnameL = wstring(xmlname.begin(), xmlname.end());
		xmlnameLPC = xmlnameL.c_str();

		cout << "What root attribute value?"  << endl;
		cout << "(ex. http://schemas.openxmlformats.org/package/2006/relationships)" << endl;
		cout << "> ";
		cin >> xmlname2;
		xmlnameL2 = wstring(xmlname2.begin(), xmlname2.end());
		xmlnameLPC2 = xmlnameL2.c_str();

		parser.Set_Attribute(xmlnameLPC, xmlnameLPC2);

		cout << "Do you want to create a child node?" << endl;
		cout << "Enter 1 for YES and 0 for NO." << endl;
		cin >> truth;
				
		while (truth == 1)
		{
			// create node
			cout << "Create the node:" << endl;
			cout << "What node? (ex. Relationship)" << endl;
			cout << "> ";
			cin >> xmlname;
			name = xmlname;
			xmlnameL = wstring(xmlname.begin(), xmlname.end());
			xmlnameLPC = xmlnameL.c_str();
			parser.Add_FirstChildNode(xmlnameLPC);


			// Create attributes for the node
			cout << "Do you want to create an attribute for " << name << " node?" << endl;
			cout << "Enter 1 for YES and 0 for NO." << endl;
			cin >> truth2;

			while (truth2 == 1)
			{
				cout << "What's the " << name << " attribute name?" << endl;
				cout << "(ex. Id)" << endl;
				cout << "> ";
				cin >> xmlname;
				xmlnameL = wstring(xmlname.begin(), xmlname.end());
				xmlnameLPC = xmlnameL.c_str();

				cout << "What's the " << name << " attribute value?" << endl;
				cout << "(ex. rId1)" << endl;
				cout << "> ";
				cin >> xmlname2;
				xmlnameL2 = wstring(xmlname2.begin(), xmlname2.end());
				xmlnameLPC2 = xmlnameL2.c_str();
				parser.Set_Attribute(xmlnameLPC, xmlnameLPC2);

				cout << "Do you want to create another attribute for " << name << " node?" << endl;;
				cout << "Enter 1 for YES and 0 for NO." << endl;
				cin >> truth;
			}

			cout << "Do you want to create another child?" << endl;
			cout << "Enter 1 for YES and 0 for NO." << endl;
			cin >> truth;
			if (truth == 1)
				parser.Go_to_Parent();
			
		}



		/*cout << "Finish " << xmlname << " node:" << endl;
		cout << "What node? (ex. Relationship)" << endl;
		cout << "> ";
		cin >> xmlname;
		xmlnameL = wstring(xmlname.begin(), xmlname.end());
		xmlnameLPC = xmlnameL.c_str();
		parser.Add_LastChildNode(xmlnameLPC);*/



		// Save xml file
		cout << "What's the path/filename.xml of the file create?" << endl;
		cout << "> ";
		cin >> xmlname;
		xmlnameL = wstring(xmlname.begin(), xmlname.end());
		parser.Save_XML_Document(xmlnameL.c_str());



	}


	return 0;
}
