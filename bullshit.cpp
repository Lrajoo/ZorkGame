//  Copyright © 2018 Lingess Rajoo. All rights reserved.
//
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "item.h"
#include "room.h"
#include "container.h"


#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
//#include "rapidxml-1.13/rapidxml_iterators.hpp"

using namespace rapidxml;
using namespace std;



int main(int argc, char * argv[])
{   
    //xml intialization for parsing
    xml_document<> doc;
    //input file to be parsed
    ifstream file("itemsample.xml");
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    string content(buffer.str());
    doc.parse<0>(&content[0]);
    //vectors for room, item, container and creature 
    vector<room> room_vector;
    vector<item> item_vector;
    vector<string> inventory_vector;
    vector<container> container_vector;
    //vector<xml_node<> * > creature_vector; 
    //root node
    xml_node<> * root_node = doc.first_node("map");
    //scanning through file and identifying rooms
    //curr = room
    for(xml_node<> * curr_node = root_node->first_node("room"); curr_node; curr_node = curr_node->next_sibling("room")){
        room roomy;
        string item_room_location;
        string container_room_location;
        if(string(curr_node->name()) == "room"){
            roomy.name = curr_node->first_node("name")->value();
            roomy.description = curr_node->first_node("description")->value();
            if(curr_node->first_node("type")){
                roomy.type = curr_node->first_node("type")->value();
            }
            for(xml_node<> * item_parse = curr_node->first_node("item"); item_parse; item_parse = item_parse->next_sibling("item")){
                    item_room_location = item_parse->value();
                    roomy.item_vector.push_back(item_room_location);
                    //cout << item_room_location << endl;
            }
            for(xml_node<> * container_parse = curr_node->first_node("container"); container_parse; container_parse = container_parse->next_sibling("container")){
                    container_room_location = container_parse->value();
                    roomy.container_vector.push_back(container_room_location);
            }
            for(xml_node<> * border = curr_node->first_node("border"); border; border = border->next_sibling("border")){
                if(string(border->value()) != "exit"){
                    if(string(border->first_node("direction")->value()) == "north"){
                        roomy.north = border->first_node("name")->value();
                    }
                    if(string(border->first_node("direction")->value()) == "south"){
                        roomy.south = border->first_node("name")->value();
                    }
                    if(string(border->first_node("direction")->value()) == "east"){
                        roomy.east = border->first_node("name")->value();
                    }
                    if(string(border->first_node("direction")->value()) == "west"){
                        roomy.west = border->first_node("name")->value();
                    }
                }
            }
            room_vector.push_back(roomy);
        }
    }
    
    //scanning through file and identifying items
    for(xml_node<> * item_node = root_node->first_node("item"); item_node; item_node = item_node->next_sibling("item")){
        item itemy;
        
        itemy.name = item_node->first_node("name")->value();
        //itemy.description = item_node->first_node("description")->value();
        if(item_node->first_node("writing")){
            itemy.writing = item_node->first_node("writing")->value();
        }
        if(item_node->first_node("turnon")){
            itemy.turn_on = item_node->first_node("turnon")->first_node("print")->value();
            itemy.action_on = item_node->first_node("turnon")->first_node("action")->value();
        }
    
        item_vector.push_back(itemy);
    }
   
    //scanning through file and identifying containers
     for(xml_node<> * container_node = root_node->first_node("container"); container_node; container_node = container_node->next_sibling("container")){
        container containery;
        containery.name = container_node->first_node("name")->value();
        
        if(container_node->first_node("item")){
            containery.item = container_node->first_node("item")->value();
        }
        if(container_node->first_node("accept")){
            containery.accept.push_back(container_node->first_node("accept")->value());
        }
        container_vector.push_back(containery);
    }
    
    int size_container = container_vector.size();
    int a = 0;
 
    int size_item = item_vector.size();
    int j = 0;

    int size_room = room_vector.size();
    int i = 0;
    string check_exit;
    string input_command;
    string curr_room_str;
    room curr_room = room_vector[0];
    room prev_room = room_vector[0];
    prev_room.getName(room_vector[0]);
    room_vector[0].getDescription(room_vector[0]);
    //curr_room.getItem(curr_room);

    while(1){
        prev_room = curr_room;
	curr_room.printName(curr_room);
	int fbs = room_vector.size();
	int rv;

	int b;
	//cout << item_name << endl;
	for(b = 0; i < fbs; i++){
	  if(room_vector[i].name == curr_room.name){
	    cout << i << endl;
	    rv = i;
	  }
        check_exit = prev_room.getType(prev_room);
        cout << ">";
        //input command
        std::getline(std::cin,input_command);
        //spilt string
        string delimiter = " ";
        string command = input_command.substr(0, input_command.find(delimiter));
        //open exit
        if(string(input_command) == "open exit" && check_exit != ""){
            cout << "Game Over" << endl;
            return 0;
        }else if(string(input_command) == "i"){
            int l = 0;
            int size_inventory = inventory_vector.size();
            if(size_inventory == 0){
                cout << "Inventory empty" << endl;
            }else{
                cout << "Inventory: ";
                for(l=0; l < size_inventory; l++){
                    if(l == 0){
                        cout << inventory_vector[l];
                    }
                    else{
                        cout << ", " << inventory_vector[l];
                    }
                    
                }
                cout << endl;
            }
        }else if(command == "turn"){
            std::size_t pos = input_command.find("on");
            std::string action_item = input_command.substr(pos);
            int length = action_item.size();
            string item_name = action_item.substr(3);
            //check if exists in inventory
            int k;
            int size_i = inventory_vector.size();
            int item_in_inventory = 0;
            int vector_num;
            for(k=0; k<size_i; k++){
                if(inventory_vector[k] == item_name){
                    item_in_inventory = 1;
                    vector_num = k;
                }
            }
            if(item_in_inventory == 1){
                int size_item_vector = item_vector.size();
                int m;
                for(m=0; m<size_item_vector; m++){
                    if(item_vector[m].name == item_name){
                        item_vector[m].turn_on_func(item_vector[m]);
			input_command = item_vector[m].action_on;
			string command = input_command.substr(0, input_command.find(delimiter));
			if(string(input_command) == "open exit" && check_exit != ""){
			  cout << "Game Over" << endl;
			  return 0;
			}else if(string(input_command) == "i"){
			  int l = 0;
			  int size_inventory = inventory_vector.size();
			  if(size_inventory == 0){
			    cout << "Inventory empty" << endl;
			  }else{
			    cout << "Inventory: ";
			    for(l=0; l < size_inventory; l++){
			      if(l == 0){
				cout << inventory_vector[l];
			      }
			      else{
				cout << ", " << inventory_vector[l];
			      }
			      
			    }
			    cout << endl;
			  }
			}else if(command == "turn"){
			  std::size_t pos = input_command.find("on");
			  std::string action_item = input_command.substr(pos);
			  int length = action_item.size();
			  string item_name = action_item.substr(3);
			  //check if exists in inventory
			  int k;
			  int size_i = inventory_vector.size();
			  int item_in_inventory = 0;
			  int vector_num;
			  for(k=0; k<size_i; k++){
			    if(inventory_vector[k] == item_name){
			      item_in_inventory = 1;
			      vector_num = k;
			    }
			  }
			  if(item_in_inventory == 1){
			    int size_item_vector = item_vector.size();
			    int m;
			    for(m=0; m<size_item_vector; m++){
			      if(item_vector[m].name == item_name){
				item_vector[m].turn_on_func(item_vector[m]);
				
			      }
			    }
			  }
			}else if(command == "put"){
			  
			  string item_name = input_command.substr(4);
			  
			}else if(command == "drop"){  
			  string item_name = input_command.substr(5);
			  //cout << item_name << endl;
			  //check if item is in inventory
			  int k;
			  int z;
			  int size_i = inventory_vector.size();
			  int size_iv = item_vector.size();
			  int item_exists = 0;
			  int vector_num;
			  int iv_num;
			  for(k=0; k<size_i; k++){
			    if(inventory_vector[k] == item_name){
			      item_exists = 1;
			      vector_num = k;
			      //cout << vector_num << endl;
			    }
			  }
			  for(z = 0; z < size_iv; z++){
			    if(item_vector[z].name == item_name){
			      iv_num = z;
			      //cout << iv_num << endl;
			    }
			  }
			  if(item_exists == 1){
			    //cout << curr_room.name << endl;
			    item_vector[iv_num].owner = curr_room.name;
			    //cout << curr_room.name << endl;
			  }
			  //add back to the room
			  //cout << "b4 push back" << endl;
			  curr_room.item_vector.push_back(inventory_vector[vector_num]);
			  //cout << "after" << endl;
			  //remove item from vector
			  inventory_vector.erase(inventory_vector.begin() + vector_num);
			  //cout << "I'm here" << endl;
			}else if(command == "take"){    //take item and adds to inventory
			  string item_name = input_command.substr(5);
			  //check if item exists in inventory
			  int k;
			  int size_i = inventory_vector.size();
			  int item_exists = 0;
			  int size_iv = item_vector.size();
			  for(k=0; k<size_i; k++){
			    if(inventory_vector[k] == item_name){
			      cout << "Item exists in inventory" << endl;
			      item_exists = 1;
			    }
			  }
			  if(curr_room.getItem(curr_room,item_name) && item_exists == 0){
			    //add to inventory
			    inventory_vector.push_back(item_name);
			    cout<< "Item " << item_name << " added to inventory" << endl;
			    for(k = 0; k < size_iv; k++){
			      if(item_vector[k].name == item_name){
				item_vector[k].owner = "inventory";
			      }
			    }
			  }
			  //item that doesn't exist
			  if(!curr_room.getItem(curr_room,item_name) && item_exists == 0) {
			    cout << "Command not recognized" << endl;
			  }
			}else if(command == "add"){
			  string delim = " ";
			  string item_name = input_command.substr(4);
			  size_t location_t = item_name.find("to");
			  string location = item_name.substr(location_t + 3);
			  item_name = item_name.substr(0, item_name.find(delim));
			  int fbs = room_vector.size();
			  //cout << item_name << endl;
			  for(int i = 0; i < fbs; i++){
			    if(room_vector[i].name == location){
			      //cout << location << endl;
			      room_vector[i].item_vector.push_back(item_name);
			    }
			  }
			  //cout << item_name << location << endl;
			}else if(string(input_command) == "n" || "s" || "e" || "w"){
			  curr_room_str = prev_room.moveRoom(prev_room, input_command);
			  if(string(curr_room_str) == "error"){
			    cout << "Command not recognized" << endl;
			  }
			  else if(prev_room.getName(prev_room) != curr_room_str){
			    for(i=0; i < size_room; i++){
			      if(string(room_vector[i].getName(room_vector[i])) == curr_room_str){
				curr_room = room_vector[i];
				curr_room.printName(curr_room);
				curr_room.getDescription(curr_room);
				//curr_room.getItem(curr_room);
				break;
			      }
			    }
			  }
			}
                    }
                }
            }
        }else if(command == "put"){

            string item_name = input_command.substr(4);
            
        }else if(command == "drop"){  
            string item_name = input_command.substr(5);
            //check if item is in inventory
            int k;
            int size_i = inventory_vector.size();
            int item_exists = 0;
            int vector_num;
            for(k=0; k<size_i; k++){
                if(inventory_vector[k] == item_name){
                    item_exists = 1;
                    vector_num = k;
                }
            }
            //add back to the room
	    cout << room_vector[rv].name << endl;
            room_vector[rv].item_vector.push_back(inventory_vector[vector_num]);
	    //cout << "hello" << endl;
            //remove item from vector
            inventory_vector.erase(inventory_vector.begin() + vector_num);
        }else if(command == "take"){    //take item and adds to inventory
            string item_name = input_command.substr(5);
            //check if item exists in inventory
            int k;
            int size_i = inventory_vector.size();
            int item_exists = 0;
            for(k=0; k<size_i; k++){
                if(inventory_vector[k] == item_name){
                    cout << "Item exists in inventory" << endl;
                    item_exists = 1;
                }
            }
            if(room_vector[rv].getItem(room_vector[rv],item_name) && item_exists == 0){
                //add to inventory
                inventory_vector.push_back(item_name);
                cout<< "Item " << item_name << " added to inventory" << endl;
		for(int x = 0; x < room_vector[rv].item_vector.size(); i++){
		  if(room_vector[rv].item_vector[i] == item_name){
		    cout << item_name << endl;
		    room_vector[rv].item_vector.erase(room_vector[rv].item_vector.begin()+x);
		  }
		}
            }
            //item that doesn't exist
            else if(!room_vector[rv].getItem(room_vector[rv],item_name) && item_exists == 0) {
                cout << "Command not recognized" << endl;
            }
        }else if(string(input_command) == "n" || "s" || "e" || "w"){
             curr_room_str = prev_room.moveRoom(prev_room, input_command);
             if(string(curr_room_str) == "error"){
                cout << "Command not recognized" << endl;
             }
             else if(prev_room.getName(prev_room) != curr_room_str){
                for(i=0; i < size_room; i++){
                    if(room_vector[i].name == curr_room_str){
                        curr_room = room_vector[i];
                        curr_room.printName(curr_room);
                        curr_room.getDescription(curr_room);
                        //curr_room.getItem(curr_room);
                        break;
                    }
                }
             }
        }        
    }

	//return 0;
}
}
