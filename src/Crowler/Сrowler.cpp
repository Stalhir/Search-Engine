#include  "Crowler.h"
#include  <iostream>
#include <future>

Crowler::Crowler(httpclient& client, indexer& idx, ThreadPool& pool, int maxDeep = 1)
: connects(client), indexer_(idx), threadPool(pool), maxDeep(maxDeep)
{

}

void Crowler::AddWork(std::string page, ParsedUrl BasicUrl, int deep)
{
    std::vector<std::string> urls = indexer_.GetHrefs(page);
    std::cout<< "Crowler::AddWork Thread id: "<< std::this_thread::get_id() << std::endl;
    for (std::string url : urls)
    {
        ParsedUrl parsed_url = indexer_.ParsingURL(url);
        parsed_url = indexer_.FixAndCheckURL(BasicUrl, parsed_url);
        threadPool.submit(std::bind(&Crowler::Work, this, parsed_url, deep+1));
    }
}


void Crowler::Work(ParsedUrl url, int deep)
{
    std::cout<< "Crowler::Work Thread id: "<< std::this_thread::get_id() << std::endl;
if (deep < maxDeep)
{
    std::cout<< "DEEP" << deep;
    std::string responce = connects.download(url.host,url.port,url.target);

    AddWork(responce,url, deep);

    indexer_.Index(responce, url);
}
else
{
std::cout << "Max deep" << std::endl;
}
}
