#include "MessageResponceMapper.h"
#include "DataPckTypes.h"

 int MessageResponceMapper::ExpectedResponce(unsigned int sentMsgType)
{
     int expectedResponce = -1;
    
    switch (sentMsgType)
    {
        case DataPckTypes::Message:
        {
            expectedResponce = (int)DataPckTypes::UserResponce;
            break;   
        }
    }
    
    return expectedResponce;
}