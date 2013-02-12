
#include "str_op.hpp"


namespace Strings
{
    std::string *stripQuotas(std::string *s)
    {
        std::string &str=*s;         //str and s are now the same

        if (str.empty()==false &&
            str[0]==str[str.length()-1] &&
            (str[0]=='"' || str[0]=='\'')
            )
        {
            str.erase(0, 1);               //remove 1st " or '
            str.erase(str.length()-1, 1);  //remove last one
        }

        return &str;
    }

    std::string stripBlanks(const std::string &s)
    {
        std::string str=s;

        //from the begining
        int len=str.length();
        int l=0;

        while (l<len && str[l]<=32)
            l++;

        if (l)
            str.erase(0, l);

        //from the end
        len=str.length();
        int p=len-1;
        while (p>=0 && str[p]<=32)
            p--;

        p++;
        if (p<len)
            str.erase(p);  //remove them all from the end

        return str;
    }
}
