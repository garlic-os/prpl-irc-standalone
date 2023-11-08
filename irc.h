/**
 * @file irc.h
 *
 * purple
 *
 * Copyright (C) 2003, 2012 Ethan Blanton <elb@pidgin.im>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301 USA
 */

#ifndef _PURPLE_IRC_H
#define _PURPLE_IRC_H

#include <glib.h>

#ifdef HAVE_CYRUS_SASL
#include <sasl/sasl.h>
#endif

#include <purple.h>

#define IRC_DEFAULT_SERVER "irc.libera.chat"
#define IRC_DEFAULT_PORT 6667
#define IRC_DEFAULT_SSL_PORT 994

#define IRC_DEFAULT_CHARSET "UTF-8"
#define IRC_DEFAULT_AUTODETECT FALSE

#define IRC_DEFAULT_QUIT "Leaving."

/* By default set the command send interval to 2 seconds and allow bursting of
 * 5 commands at once.  This means, if we haven't sent a command in 10 seconds
 * we can send 5 commands immediately with no penalty.
 */
#define IRC_DEFAULT_COMMAND_INTERVAL 2
#define IRC_DEFAULT_COMMAND_MAX_BURST 5

#define IRC_BUFSIZE_INCREMENT 1024
#define IRC_MAX_BUFSIZE 16384

#define IRC_MAX_MSG_SIZE 512

#define IRC_NAMES_FLAG "irc-namelist"

enum { IRC_USEROPT_SERVER, IRC_USEROPT_PORT, IRC_USEROPT_CHARSET };
enum irc_state { IRC_STATE_NEW, IRC_STATE_ESTABLISHED };

typedef struct {
	PurpleAccount *account;
	GHashTable *msgs;
	GHashTable *cmds;
	char *server;
	int fd;
	guint timer;
	GHashTable *buddies;

	gboolean ison_outstanding;
	GList *buddies_outstanding;

	char *inbuf;
	int inbuflen;
	int inbufused;

	GString *motd;
	GString *names;
	struct _whois {
		char *nick;
		char *real;
		char *login;
		char *ident;
		char *host;
		char *away;
		char *server;
		char *serverinfo;
		GString *channels;
		int ircop;
		int identified;
		int idle;
		time_t signon;
	} whois;
	PurpleRoomlist *roomlist;
	PurpleSslConnection *gsc;

	gboolean quitting;

	time_t recv_time;

	GQueue *send_queue;
	time_t send_time;
	guint send_handler;
	gboolean sent_partial;

	char *mode_chars;
	char *reqnick;
	gboolean nickused;
#ifdef HAVE_CYRUS_SASL
	sasl_conn_t *sasl_conn;
	const char *current_mech;
	GString *sasl_mechs;
	gboolean mech_works;
	sasl_callback_t *sasl_cb;
#endif
} IRCConn;

typedef struct {
	char *name;
	gboolean online;
	gboolean flag;
	gboolean new_online_status;
	int ref;
} IRCBuddy;

typedef int (*IRCCmdCallback
)(IRCConn *irc, const char *cmd, const char *target, const char **args);

void irc_send(IRCConn *irc, const char *buf);
void irc_send_len(IRCConn *irc, const char *buf, int len);
int irc_priority_send(IRCConn *irc, const char *buf);
gboolean irc_blist_timeout(IRCConn *irc);
gboolean irc_who_channel_timeout(IRCConn *irc);
void IRCBuddy_query(IRCConn *irc);

char *irc_escape_privmsg(const char *text, gssize length);

char *irc_mirc2html(const char *string);
char *irc_mirc2txt(const char *string);

const char *irc_nick_skip_mode(IRCConn *irc, const char *string);

gboolean irc_ischannel(const char *string);

void irc_register_commands(void);
void irc_msg_table_build(IRCConn *irc);
void irc_parse_msg(IRCConn *irc, char *input);
char *irc_parse_ctcp(
	IRCConn *irc,
	const char *from,
	const char *to,
	const char *msg,
	int notice
);
char *irc_format(IRCConn *irc, const char *format, ...);

void irc_msg_default(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_away(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_badmode(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_badnick(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_ban(IRCConn *irc, const char *name, const char *from, char **args);
void irc_msg_banfull(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_banned(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_chanmode(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_endwhois(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_features(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_invite(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_inviteonly(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_ison(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_join(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_kick(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_list(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_luser(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_mode(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_motd(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_names(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_nick(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_nickused(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_nochan(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_nonick(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_nochangenick(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_nosend(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_notice(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_notinchan(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_notop(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_part(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_ping(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_pong(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_privmsg(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_quit(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_regonly(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_time(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_topic(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_topicinfo(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_unavailable(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_unknown(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_wallops(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_whois(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_who(IRCConn *irc, const char *name, const char *from, char **args);
#ifdef HAVE_CYRUS_SASL
void irc_msg_cap(IRCConn *irc, const char *name, const char *from, char **args);
void irc_msg_auth(IRCConn *irc, char *arg);
void irc_msg_authenticate(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_authok(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_authtryagain(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
void irc_msg_authfail(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);
#endif

void irc_msg_ignore(
	IRCConn *irc,
	const char *name,
	const char *from,
	char **args
);

void irc_cmd_table_build(IRCConn *irc);

int irc_cmd_default(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_away(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_ctcp(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_ctcp_action(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_ctcp_version(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_invite(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_join(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_kick(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_list(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_mode(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_names(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_nick(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_op(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_privmsg(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_part(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_ping(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_quit(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_quote(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_query(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_remove(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_service(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_time(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_topic(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_wallops(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_whois(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);
int irc_cmd_whowas(
	IRCConn *irc,
	const char *cmd,
	const char *target,
	const char **args
);

PurpleXfer *irc_dccsend_new_xfer(PurpleConnection *gc, const char *who);
void irc_dccsend_send_file(
	PurpleConnection *gc,
	const char *who,
	const char *file
);
void irc_dccsend_recv(IRCConn *irc, const char *from, const char *msg);
#endif /* _PURPLE_IRC_H */
