#include <iostream>
#include <limits>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>

typedef unsigned int U32;

const U32 INVALID_IP = std::numeric_limits<U32>::max();

class DNSEntry
{
public:
    explicit DNSEntry(U32 ip = INVALID_IP) :
        ip_address(ip)
    {
    }

private:
    U32 ip_address;
};

class DNSCache
{
public:
    DNSEntry findEntry(const std::string &domain) const
    {
        std::cout << "Calling " << __func__ << std::endl;
        std::shared_lock<std::shared_mutex> lock(entry_mutex);
        const std::map<std::string, DNSEntry>::const_iterator iter =
            entries.find(domain);
        return (iter == entries.end()) ? DNSEntry() : iter->second;
    }

    void updateOrAddEntry(const std::string &domain, const DNSEntry &entry)
    {
        std::cout << "Calling " << __func__ << std::endl;
        std::lock_guard<std::shared_mutex> lock(entry_mutex);
        entries[domain] = entry;
    }

private:
    std::map<std::string, DNSEntry> entries;
    mutable std::shared_mutex entry_mutex;
};

int main()
{
    DNSCache dns_cache;
    const std::string domain = "Domain1";
    std::thread update_thread(&DNSCache::updateOrAddEntry,
        &dns_cache, domain, DNSEntry(1)); 
    std::thread find_thread(&DNSCache::findEntry, &dns_cache, domain);
    update_thread.join();
    find_thread.join();
}
