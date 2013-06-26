//------------------------------------------------------------------------------
// Author: Dzianis Huznou
// Description: Manager for all instances created inside analyzer module.
// Copyright (c) 2013 EPAM Systems. All Rights Reserved.
//------------------------------------------------------------------------------
#ifndef ANALYSE_MANAGER_H
#define ANALYSE_MANAGER_H
//------------------------------------------------------------------------------
#include <memory> // std::auto_ptr

#include "../auxiliary/exception.h"
#include "../auxiliary/thread.h"
#include "../auxiliary/queue.h"
#include "../controller/running_status.h"
#include "analyzers.h"
#include "nfs_data.h"
#include "nfs_parser_thread.h"
#include "print_analyzer.h"
//------------------------------------------------------------------------------
using NST::auxiliary::Exception;
using NST::auxiliary::Thread;
using NST::auxiliary::Queue;
using NST::controller::RunningStatus;
//------------------------------------------------------------------------------
namespace NST
{
namespace analyzer
{

class AnalysisManager
{
    typedef Queue<NFSData> NFSQueue;
public:
    AnalysisManager(RunningStatus& running_status) : parser_thread(NULL), queue(NULL), status(running_status)
    {
    }
    ~AnalysisManager()
    {
    }

    void print_analyzer()
    {
        std::auto_ptr<BaseAnalyzer> a(new PrintAnalyzer);
        analyzers.add(a.release());
    }

    NFSQueue& init(uint32_t q_size = 256, uint32_t q_limit = 16)
    {
        queue.reset(new NFSQueue(q_size, q_limit));
        parser_thread.reset(new NFSParserThread(*queue, analyzers, status));
        return *queue;
    }

    void start()
    {
        if(parser_thread.get())
        {
            parser_thread->create();
        }
    }

    void stop()
    {
        if(parser_thread.get())
        {
            parser_thread->stop();
        }
    }

private:
    AnalysisManager(const AnalysisManager& object);            // Uncopyable object
    AnalysisManager& operator=(const AnalysisManager& object); // Uncopyable object

    std::auto_ptr<Thread> parser_thread;
    std::auto_ptr<NFSQueue> queue;
    RunningStatus& status;
    Analyzers analyzers;
};

} // namespace analyzer
} // namespace NST
//------------------------------------------------------------------------------
#endif//ANALYSE_MANAGER_H
//------------------------------------------------------------------------------
