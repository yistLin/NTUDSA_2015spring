#include <utility>
#include <list>
#include <iostream>

struct EmptyHeap: public std::exception {};

struct process
{
	int id, p;
};

class BinomialHeap {
	private:
		/* inner class: binomial tree */
		struct BinomialTree {
			int _size;
			process element;
			std::list<BinomialTree*> children;

			BinomialTree(process _ele): _size(1), element(_ele) {
				children.clear();
			}

			/* return the size of BinomialTree
			 * #note that nullptr->size() == 0
			 * (nullptr is a null pointer (like NULL in C))
			 */
			int size() {
				return (this!=nullptr)?this->_size:0;
			}
		};
		/* some definition for convinience
		 */
		typedef BinomialTree BT;
		typedef BinomialHeap BH;
		typedef std::pair<BT*, BT*> CarrySum;
		typedef std::pair<process, BH> MaxRemainder;

		/* Merge three binomial trees which have the same sizes
		 *
		 * INPUT:   a: operand a, b: operand b, c: carry in
		 *          (each of them are either nullptr or a pointer of BinomialTree with size 2^n)
		 *
		 * OUTPUT:  a pair consist of two variables.
		 *          first: carry out (nullptr or a pointer of BinomialTree with size 2^(n+1))
		 *          second: sum (nullptr or a pointer of BinomialTree with size 2^n)
		 *
		 * (just as a full adder: http://en.wikipedia.org/wiki/Adder_%28electronics%29#Full_adder )
		 */
		CarrySum merge_tree(BT *a, BT *b, BT *c) {
			// write your code here.
			int cond = ((a->size()==0)?0:1) + ((b->size()==0)?0:1) + ((c->size()==0)?0:1);
			if (cond == 0) {
				return std::pair<BT*,BT*>(nullptr,nullptr);
			}
			else if (cond == 1) {
				BT* sum = (c->size()==0) ? ((a->size()==0)?b:a) : c;
				return std::pair<BT*,BT*>(nullptr,sum);
			}
			else if (cond == 2) {
				if (b->size() == 0) b = a;
				else if (c->size() == 0) c = a;

				if ((b->element.p) > (c->element.p)) {
					b->children.push_back(c);
					b->_size *= 2;
					return std::pair<BT*,BT*>(b,nullptr);
				}
				else {
					c->children.push_back(b);
					c->_size *= 2;
					return std::pair<BT*,BT*>(c,nullptr);
				}
			}
			else {
				if (b->element.p > c->element.p) {
					b->children.push_back(c);
					b->_size *= 2;
					return std::pair<BT*,BT*>(b,a);
				}
				else {
					c->children.push_back(b);
					c->_size *= 2;
					return std::pair<BT*,BT*>(c,a);
				}
			}
		};

		/* Pop the maximum element of a binomial tree and make other elements a binomial heap.
		 *
		 * INPUT:   a: a pointer of BinomialTree
		 *
		 * OUTPUT:  a pair consist of two variables.
		 *          first: the maximum element
		 *          second: a binomial heap consists of other elements
		 */
		MaxRemainder pop_max(BT *a) {
			// write your code here.
			BH r;
			r.size = a->size() - 1;
			process e = a->element;
			int sub_t = 0;
			while (!(a->children.empty())) {
				r.trees[sub_t++] = a->children.front();
				a->children.pop_front();
			}
			return std::pair<process,BH>(e,r);
		}

		int size;
		BT* trees[32]; //binomial trees of the binomial heap, where trees[i] is a tree with size 2^i.

	public:
		BinomialHeap(): size(0) {
			for(int i=0; i<32; ++i) trees[i] = nullptr;
		}
		BinomialHeap(process element): size(1) {
			for(int i=0; i<32; ++i) trees[i] = nullptr;
			trees[0] = new BT(element);
		}

		/* merge all elements in the binomial heap b into *this, and clear the binomial heap b.
		 *
		 * INPUT:   b: a reference of BinomialHeap
		 */
		void merge(BH &b) {
			// write your code here.
			CarrySum cs;
			BT* carryIn = nullptr;
			for (int i=0; i<32; ++i) {
				cs = merge_tree(trees[i], b.trees[i], carryIn);
				carryIn = cs.first;
				trees[i] = cs.second;
			}
			for (int i=0; i<32; ++i)
				b.trees[i] = nullptr;
			size += b.size;
			b.size = 0;
		}

		void insert(const process &element) {
			BH tmp = BH(element);
			merge(tmp);
		}

		int getSize(){
			return size;
		}

		process top() {
			int max_tree = -1;
			for (int i=0; i<32; ++i)
				if (trees[i]->size() > 0 && (max_tree == -1 || trees[i]->element.p > trees[max_tree]->element.p))
					max_tree = i;

			return trees[max_tree]->element;
		}

		process pop() {
			if(size==0) throw EmptyHeap();
			else {
				//find the tree contains maximum element
				int max_tree = -1;
				for(int i=0; i<32; ++i)
					if(trees[i]->size() > 0 && (max_tree == -1 || trees[i]->element.p > trees[max_tree]->element.p))
						max_tree = i;

				MaxRemainder m_r = pop_max(trees[max_tree]);
				process &max_element = m_r.first;
				BH &remainder = m_r.second;

				size -= trees[max_tree]->size();
				trees[max_tree] = nullptr;
				merge(remainder);
				return max_element;
			}
		}
};
