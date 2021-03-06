
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include <xestring.h>
#include <linked_list.h>
#include <stack_linked_list.h>
#include <array.h>
#include <dyn_array.h>
#include <xemap.h>

#include <string_parser.h>

#include "stack_allocator.h"

#include <btree.h>

void stack_allocator_test()
{
    StackAllocator* stack = new StackAllocator(1024);

    void *m_block = stack->get_memory_block(256, 16);
}

void bst_test()
{
    binary_search_tree bst = {};

    bst.add_node(10);
    bst.add_node(40);
    bst.add_node(20);
    bst.add_node(3);
    bst.add_node(5);
    bst.add_node(7);
    bst.add_node(8);
    bst.add_node(1);
    bst.add_node(12);
    bst.add_node(2);

    rtaverse_tree_preorder(bst.root);

    BSTreeNode *cur = find(bst.root, 12);

    BSTreeNode *min_node = min(bst.root);
    BSTreeNode *max_node = max(bst.root);
}

int str_to_int(std::string &str)
{
    int result = 0;

    for (uint32 i = 0; i < str.size(); i++)
    {
        result = result * 10 + str[i] - '0';
    }

    return result;
}

void swap(char &a, char &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

std::string int_to_str(int val)
{
    std::string result = "";
    
    int i = 0;
    while (val != 0)
    {
        int reminder = val % 10;
        result.push_back(reminder + '0');
        val /= 10;
    }

    for (int i = 0, j = result.size() - 1; i < j; i++, j--)
        swap(result[i], result[j]);

    return result;
}

int main(int argc, char **argv)
{
    std::string stri = "12345";

    int val = str_to_int(stri);

    std::string back = int_to_str(val);

    stack_allocator_test();

    bst_test();

    xestring test_append("hello");

    test_append.append(" world!!!");
    test_append.append("onemorestr!!!");

    ///////////////////////////////////
    xestring str("config");
    xestring stringnull("");
    xestring parsed_string("project xengine x64 release v01 project game x64 release project val fasihujbf fbahsjfb jhkfshak hfhajhskfh jksfah fkjha sjkfh kajsfh jkash jkfhaks hkj");
    
    string_parser::skip_whitespaces(&parsed_string);

    std::cout << parsed_string << "\n";

    char sym = str.find_at(6);
    char s = str[0];

    std::cout << str << std::endl;  

    linked_list ll;

    Node node1;
    Node node2;
    Node node3;

    node1.data = 1;
    node1.next = nullptr;
    node2.data = 2;
    node2.next = nullptr;
    node3.data = 3;
    node3.next = nullptr;

    ll.head = &node1;

    ll.insert(&node3, &node1);
    ll.insert(&node2, &node3);
    
    ll.print_all_nodes();

    stack_linked_list stl;

    stl.push(&node1);
    stl.push(&node2);
    stl.push(&node3);

    Node *erased = stl.pop_val();
    
    if(erased != nullptr)
        printf("%d\n", erased->data);


    xearray<int32> test_array;

    test_array.push_back(100);
    test_array.push_back(200);
    test_array.push_back(300);
    test_array.push_back(400);
    test_array.push_back(500);
    test_array.push_back(600);
    test_array.push_back(700);
    test_array.push_back(1000);
    test_array.push_back(1100);

    test_array.insert(1, 800);
    test_array.insert(2, 900);

    xearray<int32> art;

    art.push_back(5);
    art.push_back(2761);
    art.push_back(105);
    
    auto f = art.first();
    auto l = art.last();

    art.pop();
    art.pop();
    
    art.push_back(421);
    art.push_back(4211);
    art.push_back(4421421);

    int32 check = art[2];

    art.insert(1, 1778);
    
    xedyn_array<int32> vector;

    for (int i = 0; i < 5; ++i)
    {
        vector.push_back(i);
    }

    vector.insert(2, 21284);

    printf("Before pop\n");
    for (uint32 i = 0; i < vector.size(); i++)
    {
        printf("%d\n", vector[i]);
    }

    vector.remove(21284);
    vector.pop();

    printf("After deleting\n");
    for (uint32 i = 0; i < vector.size(); i++)
    {
        printf("%d\n", vector[i]);
    }

    int b = 3;

    return 0;
}
    
    