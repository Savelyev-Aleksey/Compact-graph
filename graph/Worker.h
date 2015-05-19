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
    void startProcess(size_t begin, size_t end);
    void interruptProcess();

    size_t getProgress() const;
    size_t getProgressEnd() const;

protected:
    void setWorker(Worker* newWorker);
    void completeProcess();
    void updateProgress(size_t current);

private:
    bool processed;
    bool interrupted;
    size_t progress;
    size_t progressEnd;
    Worker* worker;
};

#endif // WORKER_H
