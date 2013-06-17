//------------------------------------------------------------------------------
// Author: Dzianis Huznou
// Description: Manager for all instances created inside filter module.
// Copyright (c) 2013 EPAM Systems. All Rights Reserved.
//------------------------------------------------------------------------------
#ifndef FILTRATION_MANAGER_H
#define FILTRATION_MANAGER_H
//------------------------------------------------------------------------------
#include <memory> // std::auto_ptr

#include "../auxiliary/thread_group.h"
#include "../controller/running_status.h"
#include "common/dumping_processor.h"
#include "common/queueing_processor.h"
#include "pcap/packet_capture.h"
#include "pcap/packet_reader.h"
#include "processing_thread.h"
//------------------------------------------------------------------------------
using NST::controller::RunningStatus;
using NST::filter::pcap::PacketCapture;
using NST::filter::pcap::PacketReader;
using NST::auxiliary::ThreadGroup;
//------------------------------------------------------------------------------
namespace NST
{
namespace filter
{

class FiltrationManager
{
public:
    FiltrationManager(RunningStatus &s) : status(s)
    {
    }
    ~FiltrationManager()
    {
        threads.stop();
    }

    void dump_to_file(const std::string &file, const std::string &interface, const std::string &filter, int snaplen, int ms)
    {
        typedef ProcessingThread<PacketCapture, DumpingProcessor>  OnlineDumping;

        std::auto_ptr<PacketCapture>    reader    (new PacketCapture(interface, filter, snaplen, ms));
        std::auto_ptr<DumpingProcessor> processor (new DumpingProcessor(file));
        std::auto_ptr<OnlineDumping>    thread    (new OnlineDumping(reader.release(), processor.release(), status));

        threads.add(thread.release());
    }

    void capture_to_queue(/*queue*/ const std::string &interface, const std::string &filter, int snaplen, int ms)
    {
        typedef ProcessingThread<PacketCapture, QueueingProcessor> OnlineAnalyzing;

        std::auto_ptr<PacketCapture>    reader    (new PacketCapture(interface, filter, snaplen, ms));
        std::auto_ptr<QueueingProcessor>processor (new QueueingProcessor(/*queue*/));
        std::auto_ptr<OnlineAnalyzing>  thread    (new OnlineAnalyzing(reader.release(), processor.release(), status));

        threads.add(thread.release());
    }

    void read_from_file(const std::string &file)
    {
        typedef ProcessingThread<PacketReader, QueueingProcessor> OfflineAnalyzing;
        // TODO: implement Offline analyzing mode
    }

    void start()
    {
        threads.start();
    }

    void stop()
    {
        threads.stop();
    }

private:
    FiltrationManager(const FiltrationManager& object); // Uncopyable object
    FiltrationManager& operator=(const FiltrationManager& object); // Uncopyable object

private:
    ThreadGroup threads;
    RunningStatus &status;
};

} // namespace filter
} // namespace NST
//------------------------------------------------------------------------------
#endif//FILTRATION_MANAGER_H
//------------------------------------------------------------------------------
