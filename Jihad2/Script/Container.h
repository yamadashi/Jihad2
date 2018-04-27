
#ifndef YAMADA_CONTAINER
#define YAMADA_CONTAINER

# include <list>


namespace ymds {

	using namespace std;


	//Queue
	template <typename T>
	class Queue final {
	private:
		list<T> queue;

	public:
		Queue() = default;
		~Queue() = default;

		void enqueue(const T& arg) { queue.push_back(arg); }

		T dequeue() {
			T tmp = queue.front();
			queue.pop_front();
			return tmp;
		}

		void clear() { queue.clear(); }
		
		bool empty() const { return queue.empty(); }
		
		const size_t size() const { return queue.size(); }
		
		template <typename Pr>
		void remove_if(Pr pr) { queue.remove_if(pr); }

		//void foreach(const std::function<void(T&)>& func) { for (auto&& elm : queue) func(elm); }
	};


	//Stack
	template <typename T>
	class Stack final {
		private:
			list<T> stack;

		public:
			Stack() = default;
			~Stack() = default;

			void push(const T& arg) { stack.emplace_back(arg); }

			T pop() {
				T tmp = stack.back();
				stack.pop_back();
				return tmp;
			}

			void clear() { stack.clear(); }
		
			bool empty() const { return stack.empty(); }
		
			const size_t size() const { return stack.size(); }

			template <typename Pr>
			void remove_if(Pr pr) { queue.remove_if(pr); }
	};
}

#endif