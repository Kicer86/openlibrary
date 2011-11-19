//some usefull std macros

//macro for iterating over std's containers
#define FOREACH(element, set) \
    for(auto it = (set).begin(); it != (set).end();)   \
        for (auto element = *it; it != (set).end(); ++it)
