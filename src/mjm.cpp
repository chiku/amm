/*
mjm2 creates jwm's menu from many (freedesktop) desktop files automatically
Including the generated file in the rootmenu section of your system.jwmrc
Copyright (C) 2010 insmyic <gminsm@gmail.com>
Copyright (C) 2013 Chirantan Mitra <chirantan.mitra@gmail.com>
*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "menu_entry.cpp"
#include "mjm.h"


int main(int argc, char *argv[]) {

	char scandir[1024];
	char outfile[1024];
	std::string icon_extension("");
	int iitm=0, i;
	mjwm::menu_entry *menu_entries;

	strncpy(scandir, "/usr/share/applications/", 1024);
	strncpy(outfile, "./automenu", 1024);

	for (i=1; i<argc; i++) {
		if (strcmp(argv[i], "-o")==0 && i+1<argc) {strncpy(outfile, argv[i+1], 1024);}
		if (strcmp(argv[i], "-s")==0 && i+1<argc) {strncpy(scandir, argv[i+1], 1024);}
		if (strcmp(argv[i], "-a")==0 ) { icon_extension = ".png"; }
		if (strcmp(argv[i], "-h")==0 && i==1) { display_help(); exit(0); }
	}


	iitm = Reader(scandir, NULL);
	if (iitm) {
		menu_entries = new mjwm::menu_entry[iitm];
		if (menu_entries) {
			iitm = Reader(scandir, menu_entries);
			Itmsrt(iitm, menu_entries);
			Rcwrite(iitm, menu_entries, outfile, icon_extension);
			delete[] menu_entries;
		}
	} else {
		printf("read err...\n");
	}
	exit(iitm);
}


int Reader(char *scandir, mjwm::menu_entry *menu_entries) {
	DIR *dir;
	FILE *fp;
	struct dirent *dp;
	char sline[1024], stmp[1024];
	mjwm::menu_entry itmp;
	int ictr = 0, ectr = 0;
	
	dir=opendir(scandir);
	
	if (!dir) {
		ectr++;
	} else {
		itmp = mjwm::menu_entry();
		for(dp=readdir(dir);dp!=NULL;dp=readdir(dir)) {

		strcpy(stmp, "");
		strcat(stmp, scandir);
		strcat(stmp, dp->d_name);

		if ((fp = fopen(stmp, "r")) == NULL) {
			ectr++;
			break;
		}

		while (fgets(sline, 1024, fp) != NULL) {
			itmp.populate(sline);
		}

		if (itmp.is_valid()) {
			ictr++;
			if (menu_entries) {
				menu_entries[ictr-1] = itmp;
			}
		}

		fclose(fp);
		}
		closedir(dir);
	}
	if (ectr) {
		ictr = 0;
	}
	return ictr;
}


void Rcwrite(int iitm, mjwm::menu_entry *menu_entries, char *outfilename, std::string icon_extension) {
	std::ofstream file(outfilename);

	for (int i=0 ; i<iitm ; i++) {
		menu_entries[i].write_to(file, icon_extension);
	}
}


int Itmsrt(int iitm, mjwm::menu_entry *menu_entries) {
	int i, j;
	mjwm::menu_entry stmp;

	for (i=0; i<iitm-1; i++) {
		for (j=i+1; j<iitm; j++) {
			if (!menu_entries[j].has_same_name(menu_entries[i])) {
				stmp = menu_entries[i];
				menu_entries[i] = menu_entries[j];
				menu_entries[j] = stmp;
			}
		}
	}
	return 0;
}


void display_help() {
	printf("mjm2\n");
	printf("Version 1.0\n");
	printf("mjm creates jwm's menu from many (freedesktop) desktop files automatically\n");
	printf("  -o   Outfile file [Default: ./automenu]\n");
	printf("  -s   Directory to scan for '.desktop' files [Default: /usr/share/applications/]\n");
	printf("  -a   Add '.png' to icon filenames\n");
	printf("  -h   Show this help\n\n");
	printf("Including the generated file in the rootmenu section of your system.jwmrc\n");
	printf("more information -> http://github.com/chiku/mjm2\n");
}
