#ifndef _GNUHASH_H_
#define _GNUHASH_H_
#include <unordered_map>

#define vsUMap     std::unordered_map
#define vsHashComp std::hash_compare
#define vsHash     std::hash
class Unit;

namespace std
{
// #ifdef __GNUG__
template < >
struct hash< void* >
{
    hash< size_t >a;
public:
    size_t operator()( const void *key ) const noexcept
    {
        return a( (size_t) key );
    }
};
template < >
struct hash< const void* >
{
    hash< size_t >a;
public:
    size_t operator()( const void*const &key ) const noexcept
    {
        return a( (size_t) key );
    }
};
template < >
struct hash< const Unit* >
{
    hash< size_t >a;
public:
    size_t operator()( const Unit*const &key ) const noexcept
    {
        return a( (size_t) key>>4 );
    }
};
template < >
struct hash< std::pair< Unit*, Unit* > >
{
    hash< size_t >a;
public:
    size_t operator()( const std::pair< Unit*, Unit* > &key ) const noexcept
    {
        return (size_t) (size_t) ( a( (int) ( ( (size_t) key.first )>>4 ) )
                                  ^a( (int) ( ( (size_t) key.second )>>4 ) ) );
    }
};
// #endif

#ifdef __GNUG__
//Minimum declaration needed by SharedPool.h
template < class Key, class Traits = std::less< Key > >
struct hash_compare
{
public:
    static const size_t bucket_size = 4;
    static const size_t min_buckets = 8;
};
#endif

}

#endif //def _GNUHASH_H_

