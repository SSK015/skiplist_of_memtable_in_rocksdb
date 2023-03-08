/* ************************************************************************
> File Name:     InlineSkiplist.h
> Author:        Yanwen Xia
> Email: 18252677381@163.com
> Created Time:  2023年03月06日 星期一 20时16分31秒
> Description:   
 ************************************************************************/
namespace ROCKSDB_NAMESPACE {
template<class Key, class Comparator>
class InlineSkipList {
    
    struct Node; 
    struct Splice;
    struct Iterator; 

    struct NodeHeightCompare; 

    Node* const head_; 
    Node* const tail_; 
    
    const NodeHeightCompare node_height_compare_;
    Comparator const compare_;

    const int kMaxHeight;

    InlineSkipList(
        const NodeHeightCompare& node_height_compare,
        Comparator compare, int max_height);
    ~InlineSkipList();
    
    void Insert(const Key& key);

    bool Contains(const Key& key) const;

    const Key* Get(const Key& key) const;

    Iterator Seek(const Key& key) const;

    Iterator NewIterator() const;
};
}
