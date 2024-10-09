#include <cmath>

#include "Camera.hpp"

Camera::Camera(mlm::vec3 in_pos, mlm::vec3 in_up, mlm::vec3 in_front, const float &in_yaw, const float &in_pitch)
{
	this->pos = in_pos;
	this->world_up = in_up;
	this->up = in_up;
	this->front = in_front;

	this->yaw = in_yaw;
	this->pitch = in_pitch;

	this->fov = 90.0f;
	this->mouse_sensitivity = 0.02f;
	this->move_speed = 2.0f;

	this->update();
}

Camera::~Camera() {}

void Camera::update()
{
	float rad_yaw = mlm::radians(this->yaw);
	float rad_pitch = mlm::radians(this->pitch);
	this->front = mlm::normalize(mlm::vec3(
		cosf(rad_yaw) * cosf(rad_pitch),
		sinf(rad_pitch),
		sinf(rad_yaw) * cosf(rad_pitch)
	));

	this->right = mlm::normalize(mlm::cross(this->front, this->world_up));
	this->up = mlm::cross(this->right, this->front);
}

mlm::mat4	Camera::get_matrix() const
{
	return (mlm::lookat(this->pos, this->pos + this->front, this->up));
}

void 		Camera::processKeyboard(Camera_Movement direction, float &delta_time)
{
	float velocity = this->move_speed * delta_time;
	switch (direction)
	{
		case FORWARD:
		this->pos += this->front * velocity;
		break ;
	case BACKWARD:
		this->pos -= this->front * velocity;
		break ;
	case LEFT:
		this->pos -= this->right * velocity;
		break ;
	case RIGHT:
		this->pos += this->right * velocity;
		break ;
	case UP:
		this->pos += this->world_up * velocity;
		break ;
	case DOWN:
		this->pos -= this->world_up * velocity;
		break ;
	default:
		break;
	}
}

void	Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= this->mouse_sensitivity;
	yoffset *= this->mouse_sensitivity;

	this->yaw += xoffset;
	this->pitch += yoffset;

	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
		{
			this->pitch = 89.0f;
		}
		else if (this->pitch < -89.0f)
		{
			this->pitch = -89.0f;
		}
	}

	this->update();
}