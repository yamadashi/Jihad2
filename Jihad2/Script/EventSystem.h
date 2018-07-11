
#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

# include <list>


//EventManagerクラスを作って、Eventクラスを継承した処理のクラスを登録すればよい

namespace ymds {

	using namespace std;
	
	//killメソッドを提供するインターフェース
	class EventKiller {
	protected:
		bool dead;

	public:
		EventKiller() : dead(false) {}
		virtual ~EventKiller() = default;
		virtual void kill() = 0;

	};


	//具象クラスではupdateメソッドとdrawメソッド、(必要ならば)デストラクタを実装
	//update内の任意のタイミングでkillメソッドを呼ぶ
	class Event {
	private:
		EventKiller* owner_ptr;

	protected:
		void kill() { owner_ptr->kill(); }

	public:		
		Event() = default;
		virtual ~Event() = default;
		virtual void update() = 0;
		virtual void draw() const = 0;
		void setOwner(EventKiller* ptr) { owner_ptr = ptr; }

	};


	//Eventクラスのラッパークラス
	/* 抽象クラスをlistに入れることができないので(ポインタなら可能だが、扱いが複雑になる),ラッパークラスを作った */
	class EventOwner final : public EventKiller {
	private:
		Event* event;
		bool& shouldErase;

	public:
		EventOwner(Event* arg1, bool& arg2)
			:EventKiller(),
			event(arg1),
			shouldErase(arg2)
		{
			arg1->setOwner(this);
		}

		~EventOwner() { delete event; }

		void execute() { event->update(); }
		void draw() const { event->draw(); }
		bool isDead() { return dead; }
		void kill() override { dead = true; shouldErase = true; }

	};


	//Eventの管理クラス(Singleton)
	class EventManager final {
	private:
		list<EventOwner> eventList;
		bool shouldErase;
		
		EventManager()
			:shouldErase(false)
		{}
		~EventManager() = default;

	public:
		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(EventManager&&) = delete;

		static EventManager& get() { static EventManager instance; return instance; }
		
		void update() {
			for (auto& elm : eventList) elm.execute();

			if (shouldErase) {
				eventList.remove_if([](EventOwner& elm) { return elm.isDead(); }); //STLのremoveとは違い、ちゃんと削除している
				shouldErase = false;
			}
		}

		void draw() const{
			for (const auto& elm : eventList) elm.draw();
		}

		void registerEvent(Event* event) { eventList.emplace_back(event, shouldErase); }
		bool hasEvent() const { return eventList.size() > 0; }
		void releaseAllEvent() { eventList.clear(); }
	};

}


#endif EVENT_SYSTEM_H