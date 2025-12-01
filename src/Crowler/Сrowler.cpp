#include  "Crowler.h"

Crowler::Crowler(httpclient& client, indexer& idx, ThreadPool& pool)
: connects(client), indexer_(idx), threadPool(pool)
{

}


void Crowler::Work(std::vector<std::string> url, int deep)
{
std::string responce = connects.download(url.at(0),url.at(1),url.at(2));
indexer_.Index(responce, url);
}
