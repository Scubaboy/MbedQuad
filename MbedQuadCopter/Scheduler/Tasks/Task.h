#ifndef Task_H
#define Task_H

class Task
{
    public:
        virtual void ExecuteTask(int interval) = 0; 
        virtual void Init() = 0; 
};
#endif