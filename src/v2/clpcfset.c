#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "clpcfset.h"

static xmlDocPtr	g_doc = NULL;
static char			*g_encoding[16];
int		g_webmgr = 0;
int		g_messages = 0;
char	g_ip[1024];
char	g_subnet[1024];

int
main(
	int argc,
	char **argv
)
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr	new_node = NULL;
	int			ret;
	char		obj_num[16];

	if (!strcmp(argv[1], "create"))
	{
		init(argv[3]);
	}
	else if (strcmp(argv[1], "add") & strcmp(argv[1], "rpl") & strcmp(argv[1], "rmv"))
	{
		printf("invalid parameter (argv[1]: %s) \n", argv[1]);
		goto func_exit;
	}

	g_doc = xmlParseFile("clp.conf");
	strcpy(g_encoding, g_doc->encoding);

	if (!strcmp(argv[1], "create"))
	{
		add_cls(argv[2], argv[3], argv[4], argv[5]);
	}
	else if (!strcmp(argv[1], "add"))
	{
		if (!strcmp(argv[2], "srv"))
		{
			add_srv(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "hba"))
		{
			add_hba(argv[3], argv[4], argv[5], argv[6], argv[7]);
		}
		else if (!strcmp(argv[2], "device"))
		{
			if (!strcmp(argv[4], "lan"))
			{
				add_dev_lan(argv[3], argv[5], argv[6]);
			}
			else if (!strcmp(argv[4], "mdc"))
			{
				add_dev_mdc(argv[3], argv[5], argv[6]);
			}
			else if (!strcmp(argv[4], "disk"))
			{
				add_dev_diskhb(argv[3], argv[5], argv[6]);
			}
			else if (!strcmp(argv[4], "witness"))
			{
				add_dev_witnesshb(argv[3], argv[5], argv[6], argv[7]);
			}
			else if (!strcmp(argv[4], "bmc"))
			{
				add_dev_bmchb(argv[3], argv[5], argv[6]);
			}
			else if (!strcmp(argv[4], "disknp"))
			{
				add_dev_disknp(argv[3], argv[5], argv[6], argv[7]);
			}
			else if (!strcmp(argv[4], "ping"))
			{
				add_dev_pingnp(argv[3], argv[5], argv[6]);
			}
			else if (!strcmp(argv[4], "http"))
			{
				add_dev_httpnp(argv[3], argv[5], argv[6]);
			}
			else
			{
				/* error */
			}
		}
		else if (!strcmp(argv[2], "hb"))
		{
			if (!strcmp(argv[3], "lankhb"))
			{
				add_hb_lankhb(argv[4], argv[5]);
			}
			else if (!strcmp(argv[3], "lanhb"))
			{
				add_hb_lanhb(argv[4], argv[5]);
			}
			else if (!strcmp(argv[3], "diskhb"))
			{
				add_hb_diskhb(argv[4], argv[5]);
			}
			else if (!strcmp(argv[3], "bmchb"))
			{
				add_hb_bmchb(argv[4], argv[5]);
			}
			else if (!strcmp(argv[3], "witnesshb"))
			{
				add_hb_witnesshb(argv[4], argv[5], argv[6]);
			}
			else
			{
				/* error */
			}
		}
		else if (!strcmp(argv[2], "np"))
		{
			if (!strcmp(argv[3], "pingnp"))
			{
				add_np_pingnp(argv[4], argv[5], argv[6], argv[7], argv[8]);
			}
			else if (!strcmp(argv[3], "disknp"))
			{
				add_np_disknp(argv[4], argv[5]);
			}
			else if (!strcmp(argv[3], "httpnp"))
			{
				add_np_httpnp(argv[4], argv[5], argv[6]);
			}
			else
			{
				/* error */
			}
		}
		else if (!strcmp(argv[2], "alertservice"))
		{
			add_alertservice();
		}
		else if (!strcmp(argv[2], "alert"))
		{
			add_alert(argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11]);
		}
		else if (!strcmp(argv[2], "grp"))
		{
			add_grp(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "grpparam"))
		{
			add_grp_param(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "rsc"))
		{
			add_rsc(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "rscdep"))
		{
			add_rsc_dep(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "rscparam"))
		{
			add_rsc_param(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "rscguid"))
		{
			add_rsc_guid(argv[3], argv[4], argv[5], argv[6], argv[7]);
		}
		else if (!strcmp(argv[2], "mon"))
		{
			add_mon(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "monparam"))
		{
			add_mon_param(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "clsparam"))
		{
			add_cls_param(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "webmgr"))
		{
			g_webmgr = 1;
			add_cls_webmgr(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "messages"))
		{
			g_messages = 1;
			if (!strcmp(argv[3], "cmdline")) {
				add_cls_messages_cmdline(argv[4], argv[5], argv[6]);
			} else{
				add_cls_messages(argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
			}
			
		}
		else if (!strcmp(argv[2], "smtpsrv"))
		{
			add_cls_smtpsrv(argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10]);
		}
		else if (!strcmp(argv[2], "snmpsrv"))
		{
			add_cls_snmpsrv(argv[3], argv[4], argv[5], argv[6]);
		}
		else
		{

		}
	} else if (!strcmp(argv[1], "rpl")) {

	} else if (!strcmp(argv[1], "rmv")) {

	}

	ret = cal_obj_num();
	sprintf(obj_num, "%d", ret);
	add_obj_num(obj_num);

	xmlSaveFormatFileEnc("clp.conf", g_doc, g_encoding, 1);

	xmlFreeDoc(g_doc);
	xmlCleanupParser();

func_exit:
	return 0;
}


int
add_cls(
	char *clsname,
	char *charset,
	char *encode,
	char *os
)
{
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	ret = set_value(g_doc, "/root/all/charset", charset);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/all/serveros", os);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/all/encode", encode);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/cluster/name", clsname);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/cluster/comment", "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_srv(
 	char *srvname,
	char *priority
)
{
	char	path[CONF_PATH_LEN];
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	
	sprintf(path, "/root/server@%s/priority", srvname);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hba(
	char *srvname,
	char *id,
	char *port,
	char *device,
	char *instance
)
{
	char	path[CONF_PATH_LEN];
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/hba@%s/portnumber", srvname, id);
	ret = set_value(g_doc, path, port);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/hba@%s/deviceid", srvname, id);
	ret = set_value(g_doc, path, device);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/hba@%s/instanceid", srvname, id);
	ret = set_value(g_doc, path, instance);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_lan(
	char *srvname,
	char *id,
	char *ipaddr
)
{
	char 		path[CONF_PATH_LEN];
	char		type[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	i = atoi(id);
	strcpy(type, "lan");
	
	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, type);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, ipaddr);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_mdc(
	char* srvname,
	char* id,
	char* info
)
{
	char	path[CONF_PATH_LEN];
	char	*dev_number[16];
	int		i;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 400;
	i++;
	sprintf(dev_number, "%d", i);

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "mdc");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/mdc/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_diskhb(
	char* srvname, 
	char* id, 
	char* info
)
{
	char	path[CONF_PATH_LEN];
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "disk");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/extend", srvname, id);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/disk/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/disk/extend", srvname, id);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_witnesshb(
	char* srvname, 
	char* id, 
	char* info,
	char* extend
)
{
	char	path[CONF_PATH_LEN];
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "witness");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/extend", srvname, id);
	ret = set_value(g_doc, path, extend);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_bmchb(
	char* srvname,
	char* id,
	char* info
)
{
	char	path[CONF_PATH_LEN];
	int	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "bmc");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_disknp(
	char* srvname, 
	char* id, 
	char* guid, 
	char* letter
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "disknp");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, guid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/extend", srvname, id);
	ret = set_value(g_doc, path, letter);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_pingnp(
	char* srvname,
	char* id,
	char* grpid
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "ping");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, grpid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_dev_httpnp(
	char* srvname,
	char* id,
	char* info
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "http");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hb_lankhb(
	char *id,
	char *priority
)
{
	char 		path[CONF_PATH_LEN];
	char		dev_number[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = count_khb();
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@lankhb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lankhb@lankhb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lankhb@lankhb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hb_lanhb(
	char *id,
	char *priority
)
{
	char 		path[CONF_PATH_LEN];
	char		dev_number[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = count_hb();
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@lanhb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lanhb@lanhb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lanhb@lanhb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hb_bmchb(
	char *id,
	char *priority
)
{
	char 		path[CONF_PATH_LEN];
	char		dev_number[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 600;
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@bmchb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/bmchb@bmchb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/bmchb@bmchb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hb_diskhb(
	char *id,
	char *priority
)
{
	char	path[CONF_PATH_LEN];
	char	dev_number[16];
	int		i;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 300;
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@diskhb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/diskhb@diskhb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/diskhb@diskhb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hb_witnesshb(
	char *id,
	char *priority,
	char *host
)
{
	char	path[CONF_PATH_LEN];
	char	buf[CONF_PATH_LEN];
	char	*token;
	char	dev_number[16];
	char	ip[CONF_PATH_LEN];
	char	port[CONF_PATH_LEN];
	int		i;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	memset(path, "\0", sizeof(path)); 
	memset(buf, "\0", sizeof(buf)); 

	i = atoi(id) - 700;
	i++;
	sprintf(dev_number, "%d", i);

	strcpy(buf, host);
	token = strchr(buf, ':');
	if (!token)
	{
		/* no subnet mask */
		strcpy(ip, buf);
		strcpy(port, "80");
	}
	else
	{
		strncpy(ip, buf, token - buf);
		ip[token - buf] = '\0';
		strcpy(port, token + 1);	
	}
	
	sprintf(path, "/root/heartbeat/types@witnesshb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/witnesshb@witnesshb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/witnesshb@witnesshb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/witnesshb@witnesshb%s/host", dev_number);
	ret = set_value(g_doc, path, ip);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/witnesshb@witnesshb%s/port", dev_number);
	ret = set_value(g_doc, path, port);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_np_pingnp(
	char* id, 
	char* priority, 
	char* grpid, 
	char* listid, 
	char* ip
)
{
	char 	path[CONF_PATH_LEN];
	char	dev_number[16];
	int		i;
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 10200;
	i++;
	sprintf(dev_number, "%d", i);

	sprintf(path, "/root/networkpartition/types@pingnp");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/pingnp@pingnp%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/pingnp@pingnp%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/pingnp@pingnp%s/grp@%s/list@%s/ip", dev_number, grpid, listid);
	ret = set_value(g_doc, path, ip);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_np_disknp(
	char* id, 
	char* priority
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;
	char	dev_number[16];
	int		i;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 10100;
	i++;
	sprintf(dev_number, "%d", i);

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/networkpartition/types@disknp");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/disknp@disknp%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/disknp@disknp%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;

}


int
add_np_httpnp(
	char* id, 
	char* priority,
	char* host
)
{
	char	path[CONF_PATH_LEN];
	char	buf[CONF_PATH_LEN];
	char	*token;
	char	dev_number[16];
	char	ip[CONF_PATH_LEN];
	char	port[CONF_PATH_LEN];
	int		i;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	memset(path, "\0", sizeof(path)); 
	memset(buf, "\0", sizeof(buf)); 

	i = atoi(id) - 10700;
	i++;
	sprintf(dev_number, "%d", i);

	strcpy(buf, host);
	token = strchr(buf, ':');
	if (!token)
	{
		/* no subnet mask */
		strcpy(ip, buf);
		strcpy(port, "80");
	}
	else
	{
		strncpy(ip, buf, token - buf);
		ip[token - buf] = '\0';
		strcpy(port, token + 1);	
	}

	sprintf(path, "/root/networkpartition/types@httpnp");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/httpnp@httpnp%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/httpnp@httpnp%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/httpnp@httpnp%s/host", dev_number);
	ret = set_value(g_doc, path, ip);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/httpnp@httpnp%s/port", dev_number);
	ret = set_value(g_doc, path, port);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;

}

int
add_alertservice()
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	ret = set_value(g_doc, "/root/cluster/dn1000s/use", "1");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/alertservice/types@dn1000s", "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/alertservice/dn1000s@dn1000s1/priority", "0");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/alertservice/dn1000s@dn1000s1/device", "20000");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/alertservice/dn1000s@dn1000s1/kind", "nm");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_alert(
	char *srvname,
	char *id,
	char *type,
	char *info,
	char *user,
	char *password,
	char *rshpath,
	char *startvoice,
	char *stopvoice
)
{
	char 	path[CONF_PATH_LEN];
	char	nvoice[CONF_PATH_LEN];
	char	nvoicefile[CONF_PATH_LEN];
	char	evoice[CONF_PATH_LEN];
	char	evoicefile[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, type);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/user", srvname, id);
	ret = set_value(g_doc, path, user);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/password", srvname, id);
	ret = set_value(g_doc, path, password);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	
	if (!strcmp(rshpath, "0"))
	{
		sprintf(path, "/root/server@%s/device@%s/rsh", srvname, id);
		ret = set_value(g_doc, path, "0");
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
		sprintf(path, "/root/server@%s/device@%s/rshpath", srvname, id);
		ret = set_value(g_doc, path, "");
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
	}
	else
	{
		sprintf(path, "/root/server@%s/device@%s/rsh", srvname, id);
		ret = set_value(g_doc, path, "1");
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
		sprintf(path, "/root/server@%s/device@%s/rshpath", srvname, id);
		ret = set_value(g_doc, path, rshpath);
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
	}
	
	if (!strcmp(type, "dn1000s") | !strcmp(type, "patlite"))
	{
		strcpy(nvoice, "");
		strcpy(nvoicefile, "");
		strcpy(evoice, "");
		strcpy(evoicefile, "");
	}
	else if (!strcmp(type, "dn1500gl") | !strcmp(type, "nhfv1"))
	{
		if (!strcmp(startvoice, "0"))
		{
			strcpy(nvoice, "0");
			if (!strcmp(type, "dn1500gl"))
			{
				strcpy(nvoicefile, "01");
			}
			else if (!strcmp(type, "nhfv1"))
			{
				strcpy(nvoicefile, "65");
			}
		}
		else
		{
			strcpy(nvoice, "1");
			strcpy(nvoicefile, startvoice);
		}

		if (!strcmp(stopvoice, "0"))
		{
			strcpy(evoice, "0");
			if (!strcmp(type, "dn1500gl"))
			{
				strcpy(evoicefile, "02");
			}
			else if (!strcmp(type, "nhfv1"))
			{
				strcpy(evoicefile, "66");
			}
		}
		else
		{
			strcpy(evoice, "1");
			strcpy(evoicefile, stopvoice);
		}
	}
	else
	{

	}

	sprintf(path, "/root/server@%s/device@%s/normal/voice", srvname, id);
	ret = set_value(g_doc, path, nvoice);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/normal/voicefile", srvname, id);
	ret = set_value(g_doc, path, nvoicefile);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/error/voice", srvname, id);
	ret = set_value(g_doc, path, evoice);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/error/voicefile", srvname, id);
	ret = set_value(g_doc, path, evoicefile);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_grp(
	char *grptype,
	char *grpname
)
{
	char 	path[CONF_PATH_LEN];
	char	gid[16];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	ret = count_group();
	sprintf(gid, "%d", ret);

	sprintf(path, "/root/group@%s/type", grpname);
	ret = set_value(g_doc, path, grptype);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

	sprintf(path, "/root/group@%s/comment", grpname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/group@%s/gid", grpname);
	ret = set_value(g_doc, path, gid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_grp_param(
	char *grptype,
	char *grpname,
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;
	sprintf(path, "/root/group@%s/%s", grpname, tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
func_exit:
	return ret;
}


int
add_rsc(
	char *grpname,
	char *rsctype,
	char *rscname
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/group@%s/resource@%s@%s", grpname, rsctype, rscname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/resource/types@%s", rsctype);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/resource/%s@%s/comment", rsctype, rscname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_rsc_dep(
	char *rsctype,
	char *rscname,
	char *depend
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/resource/%s@%s/depend@%s", rsctype, rscname, depend);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_rsc_param(
	char *rsctype,
	char *rscname,
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

#if 0
	sprintf(path, "/root/resource/%s@%s/parameters/%s", rsctype, rscname, tag);
#endif
	sprintf(path, "/root/resource/%s@%s/%s", rsctype, rscname, tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_rsc_guid(
	char* rsctype,
	char* rscname,
	char* srvname,
	char* tag,
	char* guid
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/resource/%s@%s/server@%s/parameters/%s", rsctype, rscname, srvname, tag);
	ret = set_value(g_doc, path, guid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_mon(
	char *montype,
	char *monname
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/monitor/types@%s", montype);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/monitor/%s@%s/comment", montype, monname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_mon_param(
	char *montype,
	char *monname,
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/monitor/%s@%s/%s", montype, monname, tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_cls_param(
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;
	sprintf(path, "/root/%s", tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
func_exit:
	return ret;
}

int
add_cls_webmgr(
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	char	buf[CONF_PATH_LEN];
	char	*token;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	memset(path, "\0", sizeof(path)); 
	memset(buf, "\0", sizeof(buf)); 

	strcpy(buf, param);
	token = strchr(buf, '/');
	if (!token)
	{
		/* no subnet mask */
		strcpy(g_ip, buf);
	}
	else
	{
		strncpy(g_ip, buf, token - buf);
		g_ip[token - buf] = '\0';
		strcpy(g_subnet, token + 1);	
	}

	if (!strcmp(tag, "clientlist"))
	{
		sprintf(path, "/root/webmgr/security/clientlist/ip@%s", g_ip);
		ret = set_value(g_doc, path, NULL);
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
	}
	else if (!strcmp(tag, "clientlistro"))
	{
		sprintf(path, "/root/webmgr/security/clientlist/ipro@%s", g_ip);
		ret = set_value(g_doc, path, NULL);
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
	}

func_exit:
	return ret;
}

int
add_cls_messages(
	char *type,
	char *id,
	char *alert,
	char *altexec,
	char *mail,
	char *trap,
	char *syslog
)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/messages/types@%s", type);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/messages/%s@%s/alert", type, id);
	ret = set_value(g_doc, path, alert);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/messages/%s@%s/altexec", type, id);
	ret = set_value(g_doc, path, altexec);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/messages/%s@%s/mail", type, id);
	ret = set_value(g_doc, path, mail);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/messages/%s@%s/trap", type, id);
	ret = set_value(g_doc, path, trap);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/messages/%s@%s/syslog", type, id);
	ret = set_value(g_doc, path, syslog);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_cls_messages_cmdline(
	char *type,
	char *id,
	char *cmdline
)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	int		count;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;
	count = count_messages_cmdline(type, id, cmdline);
	if (count == -1)
	{
		printf("set_value() failed. %s already exists.\n", cmdline);
		ret = CONF_ERR_FILE;
		goto func_exit;
	}

	sprintf(path, "/root/messages/%s@%s/cmd@%d/cmdline", type, id, count);
	ret = set_value(g_doc, path, cmdline);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_cls_smtpsrv(
	char *priority,
	char *srvname,
	char *port,
	char *senderaddress,
	char *auth,
	char *authmethod,
	char *username,
	char *passwd)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	int		count;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;
	count = count_smtpsrv();

	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/priority", count);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/srvname", count);
	ret = set_value(g_doc, path, srvname);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/port", count);
	ret = set_value(g_doc, path, port);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/senderaddress", count);
	ret = set_value(g_doc, path, senderaddress);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/auth", count);
	ret = set_value(g_doc, path, auth);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/authmethod", count);
	ret = set_value(g_doc, path, authmethod);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/username", count);
	ret = set_value(g_doc, path, username);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/mail/smtp/smtpsrv@%d/passwd", count);
	ret = set_value(g_doc, path, passwd);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_cls_snmpsrv(
	char *srvname,
	char *port,
	char *snmpver,
	char *snmpcom
)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	int		count;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;
	count = count_snmpsrv();

	sprintf(path, "/root/cluster/trap/snmpsrv@%d/srvname", count);
	ret = set_value(g_doc, path, srvname);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/trap/snmpsrv@%d/port", count);
	ret = set_value(g_doc, path, port);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/trap/snmpsrv@%d/snmpver", count);
	ret = set_value(g_doc, path, snmpver);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/cluster/trap/snmpsrv@%d/snmpcom", count);
	ret = set_value(g_doc, path, snmpcom);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_obj_num(
	char *objnum
)
{
	int ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	set_value(g_doc, "/root/webmgr/client/objectnumber", objnum);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return 0;
}

int
cal_obj_num()
{
	int objnum = 0;
	int srvnum = 0;
	int htbnum = 0;
	int nprnum = 0;
	int grpnum = 0;
	int rscnum = 0;
	int monnum = 0;

	/* cluster, server, group, monitor icon */
	objnum += 4;

	srvnum = count_server();
	htbnum = count_heartbeat();
	nprnum = count_networkpartition();
	grpnum = count_group();
	rscnum = count_resource();
	monnum = count_monitor();
	objnum += srvnum + grpnum + rscnum + monnum + ((htbnum + nprnum) * srvnum);

func_exit:
	return objnum;
}

int
count_hb()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "heartbeat");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (!strcmp(n_child->name, "lanhb"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_khb()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "heartbeat");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (!strcmp(n_child->name, "lankhb"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_server()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "server");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			ret++;
		}
	}

	return ret;
}

int
count_group()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "group");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			ret++;
		}
	}

	return ret;
}

int
count_heartbeat()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "heartbeat");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_networkpartition()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "networkpartition");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_resource()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "resource");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_monitor()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "monitor");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_messages_cmdline
(
	char *type,
	char *id,
	char *cmdline
)
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n1;
	xmlNodePtr	n2;
	xmlChar		*value;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "messages");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n1 = n->children; n1 != NULL; n1 = n1->next)
			{
				if (strcmp(n1->name, type))
				{
					continue;
				}
				if (!strcmp(xmlGetProp(n1, "id"), id))
				{
					for (n2 = n1->children; n2 != NULL; n2 = n2->next)
					{
						if (!strcmp(n2->name, "cmd"))
						{
							if (!strcmp(n2->children->name, "cmdline"))
							{
								value = xmlNodeListGetString(g_doc, n2->children->children, 1);
								if (!strcmp(value, cmdline)) {
									ret = -1;
									goto func_exit;
								}
								xmlFree(value);
							}
							ret++;
						}
					}
				}
			}
		}
	}

func_exit:
	return ret;
}

int
count_smtpsrv()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n1;
	xmlNodePtr	n2;
	xmlNodePtr	n3;
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, "cluster"))
		{
			for (n1 = n->children; n1 != NULL; n1 = n1->next)
			{
				if (!strcmp(n1->name, "mail"))
				{
					for (n2 = n1->children; n2 != NULL; n2 = n2->next)
					{
						if (!strcmp(n2->name, "smtp"))
						{
							for (n3 = n2->children; n3 != NULL; n3 = n3->next)
							{
								if (!strcmp(n3->name, "smtpsrv"))
								{
									ret++;
								}
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

int
count_snmpsrv()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n1;
	xmlNodePtr	n2;
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, "cluster"))
		{
			for (n1 = n->children; n1 != NULL; n1 = n1->next)
			{
				if (!strcmp(n1->name, "trap"))
				{
					for (n2 = n1->children; n2 != NULL; n2 = n2->next)
					{
						if (!strcmp(n2->name, "snmpsrv"))
						{
							ret++;
						}
					}
				}
			}
		}
	}

	return ret;
}