#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define MAXNODES 20000
#define MAXTS 20000
#define LINEBUF 2056
#define n1topFolder 12
#define n2topFolder 21
#define n1botFolder 0
#define n2botFolder 19

int main() {

	FILE *fidNL__ = fopen("nodelist.bin", "w");
	fclose(fidNL__);
	FILE *fidCp__ = fopen("Cp.bin", "w");
	fclose(fidCp__);

	FILE *fidNL = fopen("nodelist.bin","ab+");
	FILE *fidCp = fopen("Cp.bin","ab+");

	char* cwd;
	char buff[PATH_MAX + 1];
	int ii, jj, kk, iNode;
	int iTS = 0;
	char level1folder[128], level2folder[128];
	char line[LINEBUF];

	unsigned int *nodebuff;
	nodebuff = malloc (sizeof(unsigned int) * MAXNODES);

	float *cpbuff;
	cpbuff = malloc (sizeof(float) * MAXNODES);

	//float *cpbigbuff;
	//cpbigbuff  = malloc (sizeof(float) * MAXNODES * MAXTS);

	//unsigned int *nodebigbuff;
	//nodebigbuff  = malloc (sizeof(unsigned int) * MAXNODES * MAXTS);

	DIR *dp;
	struct dirent *ep;

	cwd = getcwd( buff, PATH_MAX + 1 );

	if ( cwd != NULL ) {
		printf( "My working directory is %s\n", cwd );
	}

	for (ii = n1topFolder; ii <= n2topFolder; ii++) {

		sprintf(level1folder, "run%i", ii);

		for (jj = n1botFolder; jj <= n2botFolder; jj++) {

			sprintf(level2folder, "FEA.%i", jj);
			printf("Reading run %i FEA.%i\n", ii, jj);

			char pathfolder[PATH_MAX + 1024] = "";
			strcat(pathfolder,cwd);
			strcat(pathfolder,"/");
			strcat(pathfolder,level1folder);
			strcat(pathfolder,"/");
			strcat(pathfolder,level2folder);
			strcat(pathfolder,"/");

			dp = opendir (pathfolder);
			iNode = 0;
			if (dp != NULL) {
				while (ep = readdir (dp)) { //loop over files

					char readfile[PATH_MAX + 1024] = "";
					strcpy(readfile,pathfolder);
					strcat(readfile,ep->d_name);

					if (!strcmp(ep->d_name, ".")) {
						continue;
					}

					if (!strcmp(ep->d_name, "..")) {
						continue;
					}

					FILE *fid = fopen(readfile,"r");
					if (!fid) {
						printf("Could not open %s\n",readfile);
						continue;
					}

					rewind(fid);
					fgets(line, LINEBUF, fid); // skip header 1
					fgets(line, LINEBUF, fid); // skip header 2

					while ( fscanf(fid, "%u,%*f,%*f,%*f,%f\n", &nodebuff[iNode], &cpbuff[iNode]) ) {
						iNode++;
					}	

					fclose(fid);

				} //end loop over files 
			}

			iTS++;
			(void) closedir (dp);

			fwrite (nodebuff, sizeof(unsigned int), iNode, fidNL);
			fwrite (cpbuff, sizeof(float), iNode, fidCp);

		} //end loop over FEA directories

	} //end loop over run directories

	
	fclose(fidNL);
	fclose(fidCp);
	free(nodebuff);
	free(cpbuff);

	/* read the data

	FILE *fidCp_ = fopen("Cp.bin","rb"); 
	fread (cpbigbuff, sizeof(float), iNode*iTS, fidCp_);
	FILE *fidNL_ = fopen("nodelist.bin","rb"); 
	fread (nodebigbuff, sizeof(unsigned int), iNode*iTS, fidNL_);

	for (ii = 0; ii < iNode*iTS; ii++) {
		printf("%15u,%15.8f\n",nodebigbuff[ii],cpbigbuff[ii]);
	}

	free(cpbigbuff);
	free(nodebigbuff);
	*/

	printf("Number of nodes: %i, number of time steps: %i", iNode*iTS, iTS);
	return(0);
}
