#include <memory>
#include <string>
#include <functional>
#include <iostream>
#include <string>

struct NodeRadix
{
    using sPointer = std::shared_ptr<NodeRadix>;
    using wPointer = std::weak_ptr<NodeRadix>;

    sPointer left;
    sPointer right;
    bool dummy = true;
};

/**
 * @brief RadixTree
 */
class RadixTree
{
public:
    using sPointer = std::shared_ptr<NodeRadix>;
    using wPointer = std::weak_ptr<NodeRadix>;

    /**
     * @brief default Ctor
     */
    RadixTree():
        root(new NodeRadix)
    {}

    /**
     * @brief insert
     * @param key
     *
     * @complexity  O(h)
     */
    void insert(const std::string& key)
    {
        sPointer curr = root;
        for(auto c : key)
        {
            sPointer& next  =  (c == '0'?   curr->left    :   curr->right);
            if(!next)
                next    =   std::make_shared<NodeRadix>();
            curr = next;
        }
        curr->dummy     =   false;
    }

    /**
     * @brief print
     *
     * i.e. preorder tree walk
     */
    void print()const
    {
        std::function<void(sPointer, std::string)>
        preorder_walk  =    [&](sPointer node, std::string key)
        {
            if(node)
            {
                if(node != root   &&   !node->dummy)
                    std::cout << key << " ";

                preorder_walk(node->left,    key + '0');  
                preorder_walk(node->right,   key + '1');
            }
        };

        preorder_walk(root, std::string());
    }

private:
    sPointer root;
};


int main()
{
   RadixTree radix;

   radix.insert("1011");
   radix.insert("100");
   radix.insert("0");
   radix.insert("10");
   radix.insert("011");
   radix.insert("0000");
   radix.insert("111");
   radix.print();

   return 0;
}

