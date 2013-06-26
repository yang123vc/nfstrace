//------------------------------------------------------------------------------
// Author: Dzianis Huznou
// Description: Created for demonstration purpose only.
// Copyright (c) 2013 EPAM Systems. All Rights Reserved.
//------------------------------------------------------------------------------
#ifndef PRINT_ANALYZER_H
#define PRINT_ANALYZER_H
//------------------------------------------------------------------------------
#include <iostream>
#include <sstream>

#include "../filter/rpc/rpc_message.h"
#include "../filter/nfs/nfs_struct.h"
#include "base_analyzer.h"
#include "nfs_data.h"
//------------------------------------------------------------------------------
using namespace NST::filter::NFS3;
using namespace NST::filter::rpc;
//------------------------------------------------------------------------------
namespace NST
{
namespace analyzer
{

class PrintAnalyzer : public BaseAnalyzer
{
    typedef NFSData::Session Session;
public:
    PrintAnalyzer()
    {
    }
    virtual ~PrintAnalyzer()
    {
    }

    virtual bool call_null(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call Null" << std::endl;
        return true;
    }
    virtual bool call_getattr(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call GetAttr" << std::endl;
        return true;
    }
    virtual bool call_setattr(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call SetAttr" << std::endl;
        return true;
    }
    virtual bool call_lookup(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call LookUp" << std::endl;
        return true;
    }
    virtual bool call_access(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call Access" << std::endl;
        return true;
    }
    virtual bool call_readlink(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call ReadLink" << std::endl;
        return true;
    }
    virtual bool call_read(const Session& session, const ReadArgs& ra)
    {
        std::cout << get_session(session) << " -- NFS Call Read XID: " << ra.get_xid() << " Offset: " << ra.get_offset() << " Count: " << ra.get_count() << std::endl;
        return true;
    }
    virtual bool call_write(const Session& session, const WriteArgs& wa)
    {
        std::cout << get_session(session) << " -- NFS Call Write XID: " << wa.get_xid() << " Offset: " << wa.get_offset() << " Count: " << wa.get_count() << " Type: ";
        switch(wa.get_stable())
        {
        case 0:
            {
                std::cout << "Unstable";
            }
            break;
        case 1:
            {
                std::cout << "Data Sync";
            }
            break;
        case 2:
            {
                std::cout << "File Sync";
            }
            break;
        }
        std::cout << std::endl;
        return true;
    }
    virtual bool call_create(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call Create" << std::endl;
        return true;;
    }
    virtual bool call_mkdir(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call MKDir" << std::endl;
        return true;;
    }
    virtual bool call_symlink(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call SymLink" << std::endl;
        return true;;
    }
    virtual bool call_mknod(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call MKNod" << std::endl;
        return true;;
    }
    virtual bool call_remove(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call Remove" << std::endl;
        return true;;
    }
    virtual bool call_rmdir(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call RMDir" << std::endl;
        return true;;
    }
    virtual bool call_rename(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call Rename" << std::endl;
        return true;;
    }
    virtual bool call_link(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call Link" << std::endl;
        return true;;
    }
    virtual bool call_readdir(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call ReadDir" << std::endl;
        return true;;
    }
    virtual bool call_readdirplus(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call ReadDirPlus" << std::endl;
        return true;;
    }
    virtual bool call_fsstat(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call FSStat" << std::endl;
        return true;;
    }
    virtual bool call_fsinfo(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call FSInfo" << std::endl;
        return true;;
    }
    virtual bool call_pathconf(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call PathConf" << std::endl;
        return true;;
    }
    virtual bool call_commit(const Session& session/*, const TypeData() data*/)
    {
        std::cout << get_session(session) << " -- NFS Call Commit" << std::endl;
        return true;;
    }

private:
    std::string get_session(const Session& session) const
    {
        std::stringstream s(std::ios_base::out);
        s << "Src: " << session_addr(NFSData::Session::Source, session) << " Dst: " << session_addr(NFSData::Session::Destination, session);
        return s.str();
    }

    std::string session_addr(NFSData::Session::Direction dir, const Session& session) const
    {
        std::stringstream s(std::ios_base::out);
        switch(session.ip_type)
        {
            case NFSData::Session::v4:
                s << ipv4_string(session.ip.v4.addr[dir]);
                break;
            case NFSData::Session::v6:
                s << ipv6_string(session.ip.v6.addr[dir]);
                break;
        }
        s << ":" << session.port[dir];
        switch(session.type)
        {
            case NFSData::Session::TCP:
                s << " (TCP)";
                break;
            case NFSData::Session::UDP:
                s << " (UPD)";
                break;
        }
        return s.str();
    }

    std::string ipv6_string(const uint8_t ip[16]) const
    {
        std::stringstream address(std::ios_base::out);
        address << "IPV6";
        return address.str();
    }

    std::string ipv4_string(const uint32_t ip /*host byte order*/ ) const
    {
        std::stringstream address(std::ios_base::out);
        address << ((ip >> 24) & 0xFF);
        address << '.';
        address << ((ip >> 16) & 0xFF);
        address << '.';
        address << ((ip >> 8) & 0xFF);
        address << '.';
        address << ((ip >> 0) & 0xFF);
        return address.str();
    }
};

} // namespace analyzer
} // namespace NST
//------------------------------------------------------------------------------
#endif//PRINT_ANALYZER_H
//------------------------------------------------------------------------------
