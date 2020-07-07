#pragma once

#include <iostream>
#include <string>

#include "element.h"



/*
 *		!!!ATTENTION SHOPPERS!!!
 *	THIS IS WORK IN PROGRESS AND WILL HARM YOUR EYES AND SOUL JUST BY LOOKING AT IT
 *	=> SO IF YOU DON'T WANT TO CREATE AN HORCRUX OR BE A CRADLE FOR KURO YOU SHOULD BETTER LEAVE
 * */
class TXML_Interpreter
{
public:
	TXML_Interpreter() { root = Element("root"); }
	void run(const char* path)
	{
		FILE* file = fopen(path, "r");
		if (file==NULL) printf("no such file %s\n",path);
		Element* proc = &root;
		while (true) {
			char lh[128]; int res = fscanf(file, "%s", lh);
			if (res==EOF) break;
			else {
				std::string sproc(lh);
				if (sproc=="{") proc = 
					proc->get_next(proc->get_next_size()-1);
				else if (sproc=="}") proc = proc->get_prev();
				else proc->add_next(sproc);
			}
		}
	}
	Element* get_root() { return &root; }
private:
	Element root;
};
