#include "Worker.h"

Worker::Worker():
    processed(false),
    interrupted(false),
    progress(0u),
    progressEnd(0u),
    worker(nullptr),
    useCurrentProgress(false)
{}



Worker::~Worker()
{}



void Worker::interruptProcess()
{
    interrupted = true;
    if (worker)
        worker->interruptProcess();
    worker = nullptr;
}



bool Worker::isProcessed() const
{
    if (worker)
        worker->isProcessed();
    return processed;
}



bool Worker::isInterrupted() const
{
    if (worker)
        return worker->isInterrupted();
    return interrupted;
}



void Worker::startProcess()
{
    interrupted = false;
    processed = true;
    progress = 0u;
}



void Worker::startProcess(Worker *newWorker, unsigned begin, unsigned end)
{
    interrupted = false;
    processed = true;
    progress = begin;
    progressEnd = end;
    worker = newWorker;

    if (worker)
        worker->startProcess();
}



void Worker::startProcess(unsigned begin, unsigned end)
{
    interrupted = false;
    processed = true;
    processed = begin;
    progressEnd = end;
}



void Worker::completeProcess()
{
    if (worker)
    {
        interrupted = worker->interrupted;
        worker = nullptr;
    }
    progress = 0u;
    progressEnd = 0u;
    processed = false;
}



void Worker::updateProgress(unsigned current)
{
    progress = current;
}



void Worker::setWorker(Worker *newWorker, bool useCurrentProgress)
{
    worker = newWorker;
    worker->startProcess();
    this->useCurrentProgress = useCurrentProgress;
}



unsigned Worker::getProgress() const
{
    if (worker && !useCurrentProgress)
        return worker->getProgress();

    return progress;
}



unsigned Worker::getProgressEnd() const
{
    if (worker)
        return worker->getProgressEnd();
    return progressEnd;
}

