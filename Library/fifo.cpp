#include "fifo.hpp"

fifo::fifo(): max(60), nb(0) {}

fifo::fifo(fifo &f)
{
    this->max = f.max;
    this->nb = f.nb;
    this->queue = f.queue;
}

void fifo::push(AVPacket pkt)
{
    while(nb==max){};

    mutex_fifo.lock();
    queue.push(pkt);
    nb++;

    mutex_fifo.unlock();
}

AVPacket fifo::pop()
{
    while (nb==0){}
    mutex_fifo.lock();
    AVPacket out = queue.front();
    queue.pop();
    nb--;
    mutex_fifo.unlock();

    return out;
}

fifo::~fifo(){}