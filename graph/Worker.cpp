#include "Worker.h"

Worker::Worker():
    processed(false),
    interrupted(false),
    progress(0),
    progressEnd(0),
    worker(nullptr)
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
}



void Worker::startProcess(Worker *newWorker)
{
    interrupted = false;
    processed = true;
    progress = 0;
    worker = newWorker;
}



void Worker::completeProcess()
{
    if (worker)
    {
        progress = worker->getProgress();
        progressEnd = getProgressEnd();
        interrupted = worker->isInterrupted();
        processed = false;
        worker = nullptr;
    }
    else
        interrupted = processed = false;
}



void Worker::startProcess(size_t begin, size_t end)
{
    interrupted = false;
    processed = true;
    processed = begin;
    progressEnd = end;
}



void Worker::updateProgress(size_t current)
{
    progress = current;
}



void Worker::setWorker(Worker *newWorker)
{
    worker = newWorker;
}



size_t Worker::getProgress() const
{
    if (worker)
        return worker->getProgress();
    return progress;
}



size_t Worker::getProgressEnd() const
{
    if (worker)
        return worker->getProgressEnd();
    return progressEnd;
}

