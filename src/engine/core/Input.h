#pragma once

#include "Window.h"
#include <map>
#include <vector>
#include <functional>
#include <glm/glm.hpp>

namespace PixelBit {
	class KeyListener {
	private:
		int m_key;
		int m_action;
		std::function<void(void)> m_callback;
	public:
		KeyListener(int key, int action, std::function<void(void)> callback);
		bool fits(int key, int action);
		void run();
	};

	class ButtonListener {
	private:
		int m_button;
		int m_action;
		std::function<void(void)> m_callback;
	public:
		ButtonListener(int key, int action, std::function<void(void)> callback);
		bool fits(int key, int action);
		void run();
	};

	class Input {
	private:
		std::map<int, int> m_keys;
		std::vector<KeyListener> m_key_listeners;
		std::vector<ButtonListener> m_button_listeners;

		glm::vec2 m_cursor;
		glm::vec2 m_cursor_last;
		glm::vec2 m_cursor_motion;

		bool m_mouse_left { false };
		bool m_mouse_right { false };

		bool m_inside { false };

		float m_scroll { 0 };
		float m_scroll_motion { 0 };

		static void cursor_pos_callback(GLFWwindow* window, double x, double y);
		static void cursor_enter_callback(GLFWwindow* window, int entered);
		static void scroll_callback(GLFWwindow* window, double x, double y);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	public:
		Input(Window& window);
		KeyListener& key_listen(int key, int action, std::function<void(void)> callback);
		ButtonListener& button_listen(int button, int action, std::function<void(void)> callback);
		void listen(int key, int action, std::function<void(void)> callback);
		void remove(KeyListener& listener);
		void remove(ButtonListener& listener);
		void update();

		bool inside() const;
		bool mouse_left() const;
		bool mouse_right() const;
		glm::vec2 cursor() const;
		glm::vec2 cursor_motion() const;
		float scroll() const;
		float scroll_motion() const;

		bool pressed(int code);
	};
};