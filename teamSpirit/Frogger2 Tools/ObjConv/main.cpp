/*

	This file is part of Frogger2, (c) 1997 Interactive Studios Ltd.


	File		: main.h
	Programmer	: Matthew Cloy
	Date		: 19/11/98

----------------------------------------------------------------------------------------------- */

#include "stdio.h"
#include "string.h"
#include "math.h"

// | | | | 2048 1024 512 256  128 64 32 16 8 4 2 1

char indata[256];
char outdata[256];

int incount;
int outcount;

int threshhold = 3;
int maxlength = 7;

char inF[255],outF[255];

char typenames[10][255] = 
{
	"TYPE_NORMAL",
	"TYPE_HASPATH",
};

struct vect
{
	float x,y,z;
};

struct obj
{
	char name[32];
	float x,y,z;
	float rx,ry,rz,rv;
	char linename[32];
	float linepc;
	int type;
};

struct line
{
	char name[32];
	int nV;
	int cV;
	vect v[100];
};

long nObj = 0;
long nLine = 0;

obj objList[100];
line lineList[100];
obj cur;
line cul;

void AddLastObject(void)
{
	if (cur.name[0])
	{
		if (strncmp(cur.name, "cam", 3) != 0)
			memcpy(&objList[nObj++],&cur,sizeof(obj));

		cur.name[0]=0;
		cur.linename[0]=0;
		cur.type = 0;
	}

	if (cul.name[0])
	{
		memcpy(&lineList[nLine++],&cul,sizeof(line));
		cul.name[0]=0;
	}
}

int where=-1;

void ProcessCommand (char *line)
{
	if (!strncmp(line,"GEOMOBJECT ",8))
	{
		AddLastObject();
		where = 1;
	}
	else
	if (!strncmp(line,"SHAPEOBJECT ",12))
	{
		printf("SHAPE!\n");
		AddLastObject();
		where = 2;
	}
	else
	if (!strncmp(line,"NODE_PARENT ",12))
	{
			cur.name[0]=0;
			cul.name[0]=0;
			where = 0;
	}
	else
	if (where==1)
	{
		if (!strncmp(line,"NODE_NAME ",10))
		{
			line+=11;
			
			for (char *c = line; *c; c++)
				if (*c=='"')
				{
					*c = 0; break;
				}

			strcpy (cur.name,line);						
	
			int letr = strlen(cur.name)-1;
			while ((cur.name[letr]>='0') && (cur.name[letr]<='9')) letr--;
			letr++;
			cur.name[letr]=0;
			if (!strcmp(cur.name,"collision"))
			{
				where = 120;
				cur.name[0] = 0;
			}
		}

		if (!strncmp(line,"TM_POS ",7))
		{
			if (cur.name[0])
			{
				line+=7;
				sscanf(line,"%f	%f	%f",&cur.x,&cur.y,&cur.z);
			}
		}

		if (!strncmp(line,"GAME_PATH_LINK ",15))
		{
			cur.type = 1;
			line+=16;
			for (char *c = line; *c; c++)
				if (*c=='"')
				{
					*c = 0; break;
				}
			strcpy (cur.linename,line);
		}

		if (!strncmp(line,"GAME_PATH_PERCENT ",18))
		{
			line+=18;
			sscanf(line,"%f",&cur.linepc);
			while (cur.linepc<0) cur.linepc+=1.0;
		}

		if (!strncmp(line,"TM_ROTAXIS ",11))
		{
			if (cur.name[0])
			{
				line+=11;
				sscanf(line,"%f	%f	%f",&cur.rx,&cur.ry,&cur.rz);
			}
		}

		if (!strncmp(line,"TM_ROTANGLE ",12))
		{
			if (cur.name[0])
			{
				line+=11;
				sscanf(line,"%f",&cur.rv);			
			}
		}		
	}
	else
	if (where==2)
	{
		if (!strncmp(line,"NODE_NAME ",10))
		{
			line+=11;
			
			for (char *c = line; *c; c++)
				if (*c=='"')
				{
					*c = 0; break;
				}

			strcpy (cul.name,line);			
		}

		if (!strncmp(line,"SHAPE_VERTEXCOUNT ",18))
		{
			line+=18;
			sscanf(line,"%i",&cul.nV);
			cul.cV = 0;
		}

		
		if (!strncmp(line,"SHAPE_VERTEX_KNOT",17))
		{
			line+=17;
			while (line[0]<'0' || line [0] >'9') line++;
			while (line[0]>='0' && line [0] <='9') line++;
			line++;
			while ((line[0]<'0' || line [0] >'9') && (line[0]!='-')) line++;

			sscanf(line,"%f	%f	%f",&cul.v[cul.cV].x,&cul.v[cul.cV].y,&cul.v[cul.cV].z);
			cul.cV++;
		}
	}	
}

void ProcessLine (char *line)
{
	while ((line[0]==' ' || line[0]=='\t') && line[0]!=0) line++;
	if (line[0]=='*')
	{
		line++;
		ProcessCommand (line);
	}
}


void WriteData(void)
{
	FILE *out;
	int i,j;

	printf ("Writing: %s \n",outF);
	out = fopen (outF,"wt");

	fputs ("// Scenic object file - generated by objConv\n\n",out);
	fputs ("#include \"ultra64.h\" \n#include \"incs.h\" \n\n",out);
	for (i=0; i<nObj; i++)
		fprintf (out,"extern SCENIC Sc_%03i;\n",i);

	fprintf (out,"\n");

	for (i=0; i<nLine; i++)
		fprintf (out,"extern VECTOR Pt_%s[%i];\n",lineList[i].name,lineList[i].nV);
	
	fprintf (out,"\n");
	
	for (i=0; i<nObj; i++)
		if (objList[i].type)
			fprintf (out,"extern TYPEDATA TD_%03i;\n",i);		
	
	fprintf (out,"\n");
	
	for (i=0; i<nObj; i++)
	{
		fprintf (out,"SCENIC Sc_%03i\n =",i);
		fprintf (out,"{\n");
		fprintf (out,((i==nObj-1)?"	0, //%03i\n":"	&Sc_%03i,\n"),i+1);
		fprintf (out,"	\"%s.ndo\",\n",objList[i].name);
		fprintf (out,"	{%f,%f,%f},\n",objList[i].x,-objList[i].y,objList[i].z);
		fprintf (out,"	{%f,%f,%f,%f},\n",-objList[i].rx,objList[i].ry,-objList[i].rz,objList[i].rv);
		fprintf (out,"	%s,\n",typenames[objList[i].type]);
		fprintf (out,"	%s%s%03i,\n",
			!objList[i].type?"":"&",
			!objList[i].type?"":"TD_",
			!objList[i].type?0:i);
		fprintf (out,"};\n\n");
	}

	for (i=0; i<nLine; i++)
	{
		fprintf (out,"VECTOR Pt_%s[%i] = \n{\n",lineList[i].name,lineList[i].nV);
		for (j=0; j<lineList[i].nV; j++)
		{
			fprintf(out,"    %f,%f,%f,\n",lineList[i].v[j].x,-lineList[i].v[j].y,lineList[i].v[j].z);
		}

		fprintf (out,"};\n\n");
	}
	
	for (i=0; i<nObj; i++)
		if (objList[i].type)
		{
			long lnum = 0;
			fprintf (out,"TYPEDATA TD_%03i = \n{\n",i);		

			for (int lv = 0; lv<nLine; lv++)
				if (!strcmp (objList[i].linename,lineList[lv].name))
					lnum = lv;
				
			fprintf (out,"    %i,\n",lineList[lnum].nV);
			fprintf (out,"    Pt_%s,\n",lineList[lnum].name);
			fprintf (out,"    0,\n");
			fprintf (out,"    3,\n");
			fprintf (out,"    %i,\n",(long)(objList[i].linepc*lineList[lnum].nV));
			fprintf (out,"    0,\n");

			fprintf (out,"};\n\n");
		}
	
	fclose (out);
}


/* ----------------
	WritePSXData
	writes lovely raw data
*/

int WriteInt(FILE *f, int i)
{
	int rit = fwrite(&i, 4, 1, f);
	if (rit != 1)
		printf("Bugger!\n");
	return rit;
}

int WriteShort(FILE *f, short s)
{
	int rit = fwrite(&s, 2, 1, f);
	if (rit != 1)
		printf("Bugger!\n");
	return rit;
}

#define RECORDSIZE		28

void toPsi(char *str)
{
	char *c = str;
	int l = 0;
	
	while (*c) c++, l++;	// go to end of string
	while (l)				// step back to '.' char
	{
		c--, l--;
		if (*c == '.') { strcpy(c, ".psi"); break; }
	}
}

void WritePSXData(void)
{
	FILE *f;
	int i;

	printf ("Writing PSX data: %s \n",outF);
	f = fopen (outF,"wb");

	WriteInt(f, nObj);

	for (i=0; i<nObj; i++)
	{
		if (i<(nObj-1))
			WriteInt(f, (i+1) * RECORDSIZE);
		else
			WriteInt(f, -1);
		
		//toPsi(objList[i].name);
		strcat(objList[i].name, ".psi");
		fwrite(objList[i].name, 16, 1, f);

		WriteShort(f, (short)objList[i].x);
		WriteShort(f, (short)objList[i].z);
		WriteShort(f, (short)-objList[i].y);
		WriteShort(f, 0);
	}

	fclose (f);
}

/* ---------------- */
  
void ReadData(void)
{
	FILE *in;
	char inStr[255];

	cur.name[0]=0;
	cul.name[0]=0;
	printf ("Reading: %s \n",inF);
	in = fopen (inF,"rt");
	
	fgets(inStr,200,in);
	while (!feof(in))
	{	
		ProcessLine (inStr);
		fgets(inStr,200,in);
	}		
	fclose (in);

	AddLastObject();

	// Save+SetCurrobjName: *NODE_NAME "t2trnk"
	// SetSkip:				*NODE_PARENT "t2trnk"
	// SetPos:				*TM_POS 167.6407	-179.3794	9.0164
	// SetRot:				*TM_ROTAXIS -0.8795	-0.2375	-0.4125
	// SetRot:				*TM_ROTANGLE 1.7549

	// *NODE_PARENT "t2trnk"

}


int main (int argc, char *argv[])
{
	printf("Frogger2 Object Converter v1.1 - (c) Interactive Studios Ltd \n\n");
	if (argc < 3)
	{		
		printf("Parameters: Infile OutFile (psx)\n");
		return 1;
	}
	strcpy (inF,argv[1]);
	strcpy (outF,argv[2]);
	ReadData();

	if (argc > 3)
		WritePSXData();
	else
		WriteData();
	return 0;
}
