#if !defined(_BB_CLIENT_AUTH_H_)
#define _BB_CLIENT_AUTH_H_

#include <config.h>

#ifdef ENABLE_AUTH_BLACKBOX

#include <string>
#include <map>
#include <expat.h>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

#if defined(HAVE_CONFIG_H)
#include "../config.h"
#endif
#if defined(HAVE_PA_INTERFACE_H)
#include <pa/interface.h>
#endif

#include "http_client.h"
#include "switchcfg.h"
#include "check.h"
#include "envelope.h"
#include "bb_parser.h"


class black_box_client_auth:
        public boost::enable_shared_from_this<black_box_client_auth>,
        private boost::noncopyable

{
  public:
    
    typedef struct
    {
        std::string login_;
        std::string password_;
        std::string ip_;
        std::string session_id_;
    } auth_info_t;
        
    black_box_client_auth(boost::asio::io_service& io_service, switchcfg *_switch_cfg);
        
    ~black_box_client_auth();
        
    typedef boost::function< void (check::chk_status _status) > set_status_t;
        
    void start(const auth_info_t &_auth_info, set_status_t _status_cb);
        
    void stop();

    check_rcpt_t check_rcpt() const;
        
  protected:
    
    void report(const check::chk_status _status, const std::string &_log, bool success = true);         // end check process

    void do_stop();    
    void restart();                                                             // new BB request
    void on_error (const boost::system::error_code& ec, const std::string& logemsg);
    void on_header_read(const std::string &_headers);
    void on_response_read(const std::string &_data, bool _eof);
        
    black_box_parser m_black_box_parser;
        
    http_client_ptr m_http_client;
        
    switchcfg *m_switch_cfg;
        
    unsigned int m_connect_count;
        
    boost::asio::io_service &m_io_service;
        
    set_status_t m_set_status;
        
    std::string m_log_host;
        
    timer m_log_delay;
    
    #if defined(HAVE_PA_INTERFACE_H)        
    pa::stimer_t m_pa_timer;
    #endif
        
    boost::asio::io_service::strand m_strand;
        
    auth_info_t auth_info_;
};

typedef boost::shared_ptr<black_box_client_auth> black_box_client_auth_ptr;

#endif // ENABLE_AUTH_BLACKBOX

#endif //_BB_CLIENT_H_
