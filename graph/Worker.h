#ifndef WORKER_H
#define WORKER_H

#include <cstddef>



class Worker
{
public:
    Worker();
    ~Worker();
    bool isProcessed() const;
    bool isInterrupted() const;

    void startProcess();
    void startProcess(Worker* newWorker);
    void startProcess(unsigned begin, unsigned end);
    void interruptProcess();

    unsigned getProgress() const;
    unsigned getProgressEnd() const;

protected:
    void setWorker(Worker* newWorker);
    void completeProcess();
    void updateProgress(unsigned current);

private:
    bool processed;
    bool interrupted;
    unsigned progress;
    unsigned progressEnd;
    Worker* worker;
};

#endif // WORKER_H
