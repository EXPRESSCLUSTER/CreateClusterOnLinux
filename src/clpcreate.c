#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/tree.h>
//#include <libxml/xpath.h>
#include <libxml/parser.h>
//#include <libxml/xpathInternals.h>

#include "clpcreate.h"

static xmlDocPtr	g_doc = NULL;
static char			*g_encoding[16];

int
main(
	int argc,
	char **argv
)
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr	new_node = NULL;
	int			ret;

	if (!strcmp(argv[1], "init"))
	{
		init(argv[2]);
		goto func_exit;
	}

	g_doc = xmlParseFile("clp.conf");
	strcpy(g_encoding, g_doc->encoding);

	if (!strcmp(argv[1], "add"))
	{
		if (!strcmp(argv[2], "cls"))
		{
			add_cls(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "srv"))
		{
			add_srv(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "ip"))
		{
			add_ip(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "hb"))
		{
			add_hb(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "objnum"))
		{
			add_obj_num(argv[3]);
		}
	}
	else
	{
		printf("invalid parameter (argv[1]: %s) \n", argv[1]);
	}

	xmlSaveFormatFileEnc("clp.conf", g_doc, g_encoding, 1);

	xmlFreeDoc(g_doc);
	xmlCleanupParser();

func_exit:
	return 0;
}


int
add_cls
(
	char *clsname,
	char *lang,
	char *os
)
{
	set_value(g_doc, "/root/all/charset", lang);
	set_value(g_doc, "/root/all/serveros", os);
	set_value(g_doc, "/root/all/encode", lang);
	set_value(g_doc, "/root/cluster/name", clsname);
	set_value(g_doc, "/root/cluster/comment", "");

func_exit:
	return 0;
}


int
add_srv
(
	char *srvname,
	char *priority
)
{
	char 		path[1024];
	
	strcpy(path, "/root/server@");
	strcat(path, srvname);
	strcat(path, "/priority");
	set_value(g_doc, path, priority);

func_exit:
	return 0;
}


int
add_ip
(
	char *srvname,
	char *id,
	char *ipaddr
)
{
	char 		path[1024];
	
	strcpy(path, "/root/server@");
	strcat(path, srvname);
	strcat(path, "/device@");
	strcat(path, id);
	strcat(path, "/type");
	set_value(g_doc, path, "lan");
	strcpy(path, "/root/server@");
	strcat(path, srvname);
	strcat(path, "/device@");
	strcat(path, id);
	strcat(path, "/info");
	set_value(g_doc, path, ipaddr);
	//need to create additional func for reducing redundancy

func_exit:
	return 0;
}


int
add_hb
(
	char *id,
	char *priority
)
{
	char 		path[1024];
	char		dev_number[16];
	int			i;

 
	i = atoi(id);
	i++;
	sprintf(dev_number, "%d", i);
	
	set_value(g_doc, "/root/heartbeat/types@lankhb", "");
	strcpy(path, "/root/heartbeat/lankhb@lankhb");
	strcat(path, dev_number);
	strcat(path, "/priority");
	set_value(g_doc, path, priority);
	strcpy(path, "/root/heartbeat/lankhb@lankhb");
	strcat(path, dev_number);
	strcat(path, "/device");
	set_value(g_doc, path, id);

func_exit:
	return 0;
}


int
add_obj_num
(
	char *objnum
)
{
	set_value(g_doc, "/root/webmgr/client/objectnumber", objnum);

func_exit:
	return 0;
}