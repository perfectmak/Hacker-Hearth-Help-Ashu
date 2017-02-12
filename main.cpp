#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    int odd;
    int even;
    int leaf;
};
vector<Node> tree;
vector<long> items;

bool isEven(long l) {
    return (l % 2) == 0;
}

Node merge(Node left, Node right) {
    Node n = {0, 0, -1};
    n.even = left.even + right.even;
    n.odd = left.odd + right.odd;
    return n;
}

void build(int node, int start, int end)
{
    if(start == end)
    {
        // Leaf node will have a single element
        Node n = {0, 0, start};
        if(isEven(items[start])) {
            n.even = 1;
        } else {
            n.odd = 1;
        }
        tree[node] = n;
    }
    else
    {
        int mid = (start + end) / 2;
        // Recurse on the left child
        build(2*node, start, mid);
        // Recurse on the right child
        build(2*node+1, mid+1, end);
        // Internal node will have the sum of both of its children

        tree[node] = merge(tree[2*node], tree[2*node+1]);
//        tree[node] = tree[2*node] + tree[2*node+1];
    }
}


void update(int node, int start, int end, int idx, long val)
{
    if(start == end)
    {
        // Leaf node
        items[idx] = val;
        Node n = {0, 0, start};
        if(isEven(items[idx])) {
            n.even = 1;
        } else {
            n.odd = 1;
        }
        tree[node] = n;
    }
    else
    {
        int mid = (start + end) / 2;
        if(start <= idx and idx <= mid)
        {
            // If idx is in the left child, recurse on the left child
            update(2*node, start, mid, idx, val);
        }
        else
        {
            // if idx is in the right child, recurse on the right child
            update(2*node+1, mid+1, end, idx, val);
        }
        // Internal node will have the sum of both of its children
        tree[node] = merge(tree[2*node], tree[2*node+1]);
//        tree[node] = tree[2*node] + tree[2*node+1];
    }
}

void updateRange(int node, int start, int end, int l, int r, int val)
{
    // out of range
    if (start > end or start > r or end < l)
        return;

    // Current node is a leaf node
    if (start == end)
    {
        // Add the difference to current node
        Node n = {0, 0, start};
        if(isEven(val)) {
            n.even = 1;
        } else {
            n.odd = 1;
        }
        tree[node] = merge(tree[node], n);
//        tree[node] += val;
        return;
    }

    // If not a leaf node, recur for children.
    int mid = (start + end) / 2;
    updateRange(node*2, start, mid, l, r, val);
    updateRange(node*2 + 1, mid + 1, end, l, r, val);

    // Use the result of children calls to update this node
    tree[node] = merge(tree[2*node], tree[2*node+1]);
//    tree[node] = tree[node*2] + tree[node*2+1];
}


Node query(int node, int start, int end, int l, int r)
{
    if(r < start or end < l)
    {
        // range represented by a node is completely outside the given range
        return Node();
    }
    if(l <= start and end <= r)
    {
        // range represented by a node is completely inside the given range
        return tree[node];
    }
    // range represented by a node is partially inside and partially outside the given range
    int mid = (start + end) / 2;
    Node p1 = query(2*node, start, mid, l, r);
    Node p2 = query(2*node+1, mid+1, end, l, r);
    return merge(p1, p2);
//    return (p1 + p2);
}

string to_string(Node n) {
    return "{" + to_string(n.odd) + ", " + to_string(n.even) + ", " + to_string(n.leaf) + "]";
}

ostream& operator<<(ostream& out, Node n) {
    out << to_string(n);
    return out;
}

template<typename T>
string toString(vector<T> v) {
    string result = "";
    for(auto i : v) {
        result += to_string(i) + ", ";
//        cout << i << ", ";
    }
//    cout << endl;
    return result;
}

/**
 * Tree size has to be a power of 2 that is greater than 2*n
 * @param n
 * @return
 */
unsigned int getTreeSize(int n) {
    unsigned int size = 1;
    while(true) {
        if(size > (2*n)) {
            break;
        }
        size = size << 1;
    }

    return size;
}

int main() {
    int n, q;
    cin >> n;
    items = vector<long>(n, 0l);
    tree = vector<Node>(getTreeSize(n), {0, 0});
    for (int i = 0; i < n; ++i) {
        cin >> items[i];
    }
    build(1, 0, n-1);

    cin >> q;
    int a, x;
    long y;
    for (int i = 0; i < q; ++i) {
        cin >> a >> x >> y;
        switch(a) {
            case 0:
                update(1, 0, n-1, x-1, y);
                break;
            case 1:
                //get even number in range
                cout << query(1, 0, n-1, x, y).even << endl;
                break;
            case 2:
                //get odd numbers in range
                cout << query(1, 0, n-1, x, y).odd << endl;
                break;
        }
    }

    return 0;
}

/**
 * Code for Lazy Update Commented away for now :)
 *
 */
//void updateRange(int node, int start, int end, int l, int r, int val)
//{
//    if(lazy[node] != 0)
//    {
//        // This node needs to be updated
//        tree[node] += (end - start + 1) * lazy[node];    // Update it
//        if(start != end)
//        {
//            lazy[node*2] += lazy[node];                  // Mark child as lazy
//            lazy[node*2+1] += lazy[node];                // Mark child as lazy
//        }
//        lazy[node] = 0;                                  // Reset it
//    }
//    if(start > end or start > r or end < l)              // Current segment is not within range [l, r]
//        return;
//    if(start >= l and end <= r)
//    {
//        // Segment is fully within range
//        tree[node] += (end - start + 1) * val;
//        if(start != end)
//        {
//            // Not leaf node
//            lazy[node*2] += val;
//            lazy[node*2+1] += val;
//        }
//        return;
//    }
//    int mid = (start + end) / 2;
//    updateRange(node*2, start, mid, l, r, val);        // Updating left child
//    updateRange(node*2 + 1, mid + 1, end, l, r, val);   // Updating right child
//    tree[node] = tree[node*2] + tree[node*2+1];        // Updating root with max value
//}
//
//int queryRange(int node, int start, int end, int l, int r)
//{
//    if(start > end or start > r or end < l)
//        return 0;         // Out of range
//    if(lazy[node] != 0)
//    {
//        // This node needs to be updated
//        tree[node] += (end - start + 1) * lazy[node];            // Update it
//        if(start != end)
//        {
//            lazy[node*2] += lazy[node];         // Mark child as lazy
//            lazy[node*2+1] += lazy[node];    // Mark child as lazy
//        }
//        lazy[node] = 0;                 // Reset it
//    }
//    if(start >= l and end <= r)             // Current segment is totally within range [l, r]
//        return tree[node];
//    int mid = (start + end) / 2;
//    int p1 = queryRange(node*2, start, mid, l, r);         // Query left child
//    int p2 = queryRange(node*2 + 1, mid + 1, end, l, r); // Query right child
//    return (p1 + p2);
//}