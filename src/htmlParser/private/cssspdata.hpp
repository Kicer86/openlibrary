
/***************************************
 *       Css parser private data       *
 *      Author: Michał Walenciak       *
 ***************************************/

#ifndef OPENLIBRARY_CSSSPDATA_HPP
#define OPENLIBRARY_CSSSPDATA_HPP

#include "private/searchlist.hpp"

class HtmlTagList;


struct CssSPData  //CssSelectorParserData
{
        CssSPData(): htmlCode(0), results() {}

        const HtmlTagList *htmlCode;    //pointer to list of tags
        SearchList results;             //list of search results

    private:
        CssSPData(const CssSPData &css);
        void operator=(const CssSPData &css);
};

#endif
