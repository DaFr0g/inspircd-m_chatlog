/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2009 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *          the file COPYING for details.
 *
 * Channel Chat Logging module (c) 2015 Daniel Rich <drich@employees.org>
 *
 * ---------------------------------------------------
 */

#include "inspircd.h"

/* $ModDesc: A module which logs all chat to the ircd log at default loglevel. */

class ModuleChatLog : public Module
{
 private:
    std::vector<std::string> logexception;
 public:
    void init()
    {
	OnRehash(NULL);
        Implementation eventlist[] = { I_OnRehash, I_OnUserPreMessage, I_OnUserPreNotice, I_OnUserJoin, I_OnUserPart, I_OnUserQuit, I_OnUserKick };
        ServerInstance->Modules->Attach(eventlist, this, sizeof(eventlist)/sizeof(Implementation));
    }

    virtual ~ModuleChatLog()
    {
    }

    void OnRehash(User* user)
    {
        logexception.clear();

        ConfigTagList tags = ServerInstance->Config->ConfTags("chatlog");
        for (ConfigIter i = tags.first; i != tags.second; ++i)
            logexception.push_back(i->second->getString("exception"));
    }

    virtual ModResult OnUserPreMessage(User* user,void* dest,int target_type, std::string &text, char status, CUList &exempt_list)
    {
        if (target_type == TYPE_USER)
        {
            User* u = (User*)dest;
            // Exclude messages for excluded nicks - both to and response
            for (std::vector<std::string>::iterator x = logexception.begin(); x != logexception.end(); x++) {
                if (InspIRCd::Match(u->nick.c_str(), *x, ascii_case_insensitive_map))
                    return MOD_RES_PASSTHRU;
                if (InspIRCd::Match(user->nick.c_str(), *x, ascii_case_insensitive_map))
                    return MOD_RES_PASSTHRU;
            }

            ServerInstance->Logs->Log("m_chatlog_user",DEFAULT,"%s: %s <%s@%s> %s",u->nick.c_str(), user->nick.c_str(), user->ident.c_str(), user->host.c_str(), text.c_str());
         }
         else if (target_type == TYPE_CHANNEL)
         {
              Channel* c = (Channel*)dest;

              ServerInstance->Logs->Log("m_chatlog_channel",DEFAULT,"%s: %s <%s@%s> %s",c->name.c_str(), user->nick.c_str(), user->ident.c_str(), user->host.c_str(), text.c_str());
         }
         return MOD_RES_PASSTHRU;
    }


    ModResult OnUserPreNotice(User* user,void* dest,int target_type, std::string &text, char status, CUList &exempt_list)
    {
         return OnUserPreMessage(user,dest,target_type,text,status,exempt_list);
    }


    void OnUserJoin(Membership* memb, bool sync, bool created, CUList& excepts)
    {
         ServerInstance->Logs->Log("m_chatlog_channel",DEFAULT,"%s: *** %s <%s@%s> has joined %s", memb->chan->name.c_str()), memb->user->nick.c_str(), memb->user->ident.c_str(), memb->user->host.c_str(), memb->chan->name.c_str());
    }

    void OnUserPart(Membership* memb, std::string &partmessage, CUList& except_list)
    {
         ServerInstance->Logs->Log("m_chatlog_channel",DEFAULT,"%s: *** %s <%s@%s> has left %s: %s", memb->chan->name.c_str(), memb->user->nick.c_str(), memb->user->ident.c_str(), memb->user->host.c_str(), memb->chan->name.c_str(), partmessage.c_str());
    }


    void OnUserQuit(User* user, Membership* memb, const std::string& message, const std::string& oper_message)
    {
         ServerInstance->Logs->Log("m_chatlog_channel",DEFAULT,"%s: *** %s <%s@%s> has quit IRC: %s", memb->chan->name.c_str(), user->nick.c_str(), user->ident.c_str(), user->host.c_str() ,message.c_str());
    }

    void OnUserKick(User* source, Membership* memb, const std::string &reason, CUList& excepts)
    {
         ServerInstance->Logs->Log("m_chatlog_channel",DEFAULT,"%s: *** %s <%s@%s> has been kicked by %s <%s@%s> from %s: %s", memb->chan->name.c_str(), memb->user->nick.c_str(), memb->user->ident.c_str(), memb->user->host.c_str(), source->nick.c_str(), source->ident.c_str(), source->host.c_str(), memb->chan->name.c_str(), reason.c_str());
    }

    virtual Version GetVersion()
    {
        return Version("A module which logs all chat to the ircd log at default loglevel.",VF_VENDOR);
    }

};

MODULE_INIT(ModuleChatLog)
