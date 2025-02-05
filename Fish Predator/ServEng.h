#pragma once

#ifdef SERVENG_EXPORTS 
#define SERVENG_API __declspec(dllexport)
#else
#define SERVENG_API __declspec(dllimport)
#endif

#include <random>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

enum class dirs { left = 0, right = 1, up = 2, down = 3, stop = 4 };

struct FPOINT
{
	float x{ 0 };
	float y{ 0 };
};

// GAME ACTIVE CREATURES

constexpr uint16_t no_creature{ 0 };
constexpr uint16_t fish1{ 0b0000000000000001 };
constexpr uint16_t fish2{ 0b0000000000000010 };
constexpr uint16_t fish3{ 0b0000000000000100 };
constexpr uint16_t fish4{ 0b0000000000001000 };
constexpr uint16_t fish5{ 0b0000000000010000 };
constexpr uint16_t fish6{ 0b0000000000100000 };
constexpr uint16_t fish7{ 0b0000000001000000 };
constexpr uint16_t fish8{ 0b0000000010000000 };
constexpr uint16_t fish9{ 0b0000000100000000 };

constexpr uint16_t hero{ 0b0000001000000000 };

// BACKGROUND CREATURES

constexpr uint16_t bubbles{ 0b0000010000000000 };
constexpr uint16_t grass{ 0b0000100000000000 };
constexpr uint16_t jelly1{ 0b0001000000000000 };
constexpr uint16_t jelly2{ 0b0010000000000000 };

namespace dll
{
	class SERVENG_API RANDiT
	{
		private:
			std::random_device rd{};
			std::seed_seq* sq{ nullptr };
			std::mt19937* twister{ nullptr };

		public:

			RANDiT();
			~RANDiT();

			int operator() (int min, int max);
	};

	class SERVENG_API PROTON
	{
		protected:
			float width{ 0 };
			float height{ 0 };
			PROTON* mPtr{ nullptr };

		public:
			FPOINT start{};
			FPOINT end{};
			FPOINT center{};

			PROTON(float _init_x = 0, float _init_y = 0, float _init_width = 1.0f, float _init_height = 1.0f);
			PROTON(float _init_x, float _init_y, float _init_width, float _init_height, PROTON*& heap_pointer);

			virtual ~PROTON();

			PROTON* GetThis() const;
			float GetWidth() const;
			float GetHeight() const;

			void SetEdges();
			void SetWidth(float _new_width);
			void SetHeight(float _new_height);
			void SetNewDims(float _new_width, float _new_height);
	};

	class SERVENG_API BASE_OBJECT :public PROTON
	{
		protected:
			uint16_t type{ no_creature };
			float move_sx{ 0 };
			float move_sy{ 0 };
			float move_ex{ 0 };
			float move_ey{ 0 };

			float slope{ 0 };
			float intercept{ 0 };

			bool hor_line = false;
			bool vert_line = false;

			void SetPathInfo(float _ex, float _ey);
			float Distance(FPOINT targ_point, FPOINT my_point);

			BASE_OBJECT(uint16_t _what, float _wherex, float _wherey);
	
			float speed{ 0 };
			int frame = 0;
			int max_frames = 0;
			int frame_delay = 0;

		public:

			int strenght = 0;
			dirs dir = dirs::stop;

			virtual ~BASE_OBJECT() {};

			int GetFrame();
			uint16_t GetType() const;

			virtual FPOINT AINextMove(FPOINT hero) = 0;
			virtual bool Move(float gear, float to_where_x = 0, float to_where_y = 0) = 0;
			virtual void Release() = 0;
	};

	class SERVENG_API EVILS :public dll::BASE_OBJECT
	{
	protected:
		EVILS(uint16_t _what_evil, float put_x, float put_y);

	public:
		friend SERVENG_API BASE_OBJECT* ObjectFactory(uint16_t what, float first_x, float first_y);
		
		FPOINT AINextMove(FPOINT hero) override;
		bool Move(float gear, float to_where_x = 0, float to_where_y = 0) override;
		void Release() override;
	};

	class SERVENG_API HERO :public dll::BASE_OBJECT
	{
	protected:
		HERO(uint16_t _what_evil, float put_x, float put_y);

	public:
		friend SERVENG_API BASE_OBJECT* ObjectFactory(uint16_t what, float first_x, float first_y);

		FPOINT AINextMove(FPOINT hero) override;
		bool Move(float gear, float to_where_x = 0, float to_where_y = 0) override;
		void Release() override;
	};

	class SERVENG_API ASSETS :public dll::BASE_OBJECT
	{
	protected:
		ASSETS(uint16_t _what_evil, float put_x, float put_y);

	public:
		friend SERVENG_API BASE_OBJECT* ObjectFactory(uint16_t what, float first_x, float first_y);

		FPOINT AINextMove(FPOINT hero) override;
		bool Move(float gear, float to_where_x = 0, float to_where_y = 0) override;
		void Release() override;
	};

	using Object = BASE_OBJECT*;

	SERVENG_API BASE_OBJECT* ObjectFactory(uint16_t what, float first_x, float first_y);
}