#ifndef _JSON_HELPER
#define _JSON_HELPER

#include <bits/stdc++.h>

#include "../ThirdParty/json/json/json.h"

std::string Path = "./";

void ReadJson(std::string fileName)
{
    Json::Reader reader;
    Json::Value root;

    std::ifstream in(Path + fileName, std::ios::binary);

    if (!in.is_open())
    {
        std::cout << "Error opening" << std::endl;
        return;
    }

    if (reader.parse(in, root))
    {
        std::vector<std::string> keys = root.getMemberNames();
        for (int i = 0; i < root.size(); i++) 
        {
            std::cout << keys[i] << ":" << root[keys[i]].asString() << std::endl;
        }
    }

    in.close();
}

int main()
{
    return 0;
}

#endif