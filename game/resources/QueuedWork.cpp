#include "QueuedWork.h"

std::deque<Queued *> QueuedWork::ResourceLoading, QueuedWork::Background;
std::vector<Queued *> QueuedWork::Polls; 


