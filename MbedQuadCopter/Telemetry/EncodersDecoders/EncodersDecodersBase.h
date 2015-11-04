#ifndef EncodersDecodersBase_H
#define EncodersDecodersBase_H

class EncodersDecodersBase
{
    public:
        virtual void Encode() = 0;
    private:
        virtual void Decode() = 0;
};
#endif