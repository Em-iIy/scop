#pragma once

#include <emlm/emlm.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Camera {
	private:
		mlm::vec3	world_up;
		mlm::vec3	pos;
		mlm::vec3	up;
		mlm::vec3	right;
		mlm::vec3	front;

		float		yaw;
		float		pitch;

		float		fov;
		float		move_speed;
		float		mouse_sensitivity;

		void update();

	public:
		Camera(mlm::vec3 in_pos = mlm::vec3(0.0f), mlm::vec3 in_up = mlm::vec3(0.0f, 1.0f, 0.0f), mlm::vec3 in_front = mlm::vec3(0.0f, 0.0f, 1.0f), const float &in_yaw = -90.0f, const float &in_pitch = 0.0f);
		~Camera();

		mlm::mat4	get_matrix() const;
		void 		processKeyboard(Camera_Movement direction, float &delta_time);
		void		processMouseMovement(float xoffset, float yoffset, bool constrainPitch);
};