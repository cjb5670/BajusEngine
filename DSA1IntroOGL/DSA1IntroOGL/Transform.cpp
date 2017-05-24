#include "Transform.h"
#include <math.h>


Transform::Transform()
{
	direction = vec3(0, 0, 1);
}


Transform::~Transform()
{
}

void Transform::SetForces(float _mass, float _maxSpeed)
{
	mass = _mass;
	maxSpeed = _maxSpeed;
	acceleration = vec3(0, 0, 0);
}

void Transform::SetWander(float _jitter, float _turnRadius, float _wanderAngle)
{
	jitter = _jitter;
	turnRadius = _turnRadius;
	wanderAngle = _wanderAngle;
}

void Transform::updateFriend(float _dt)
{
	CameraFollow(_dt);
	ApplyFriction(2);
	UpdatePosition(_dt);
	UpdateTransform(_dt);
}

void Transform::updateEnemy(float _dt)
{
	ApplyForce(Wandering());
	ApplyFriction(2);
	UpdatePosition(_dt);
	UpdateTransform(_dt);
	
}

void Transform::updateMissile(float _dt)
{
	UpdatePosition(_dt);
	UpdateTransform(_dt);
}

void Transform::ApplyForce(vec3 force)
{
	acceleration += (force / mass);
}

void Transform::ApplyRotate(float yaw, float pitch, float roll, float dt)
{
	rotation.x += yaw * dt;
	rotation.y = roll;
	rotation.z = pitch;
}

void Transform::UpdatePosition(float _dt)
{
	position = location;

	velocity += acceleration * _dt;

	position += velocity * _dt;

	prevDirection = direction;

	direction = normalize(velocity);

	acceleration = vec3(0, 0, 0);
}

void Transform::UpdateTransform(float _dt)
{
	if (sqrt((velocity.x*velocity.x) + (velocity.y*velocity.y) + (velocity.z*velocity.z)) <= maxSpeed)
	{
		location = location + (velocity * _dt);
	}
	else
	{
		location = location + ((normalize(velocity) * maxSpeed) * _dt);
	}
	
	// trying to update rotation to direction craft is facing.
	float A = (rotation.x*direction.x) + (rotation.y*direction.y) + (rotation.z*direction.z);
	float B = sqrtf((rotation.x*rotation.x) + (rotation.y*rotation.y) + (rotation.z*rotation.z));
	float C = sqrtf((direction.x*direction.x) + (direction.y*direction.y) + (direction.z*direction.z));
	//rotation = vec3(rotation.x - acos(A / (B*C)), rotation.y, rotation.z);

	float D = atan2(prevDirection.x, direction.x);
	//rotation = rotation + vec3(D, rotation.y, rotation.z);


	/*rotation = rotation + (acos(
		((rotation.x*direction.x) + (rotation.y*direction.y) + (rotation.z*direction.z)) /
		(sqrtf((rotation.x*rotation.x) + (rotation.y*rotation.y) + (rotation.z*rotation.z))) *
		(sqrtf((direction.x*direction.x) + (direction.y*direction.y) + (direction.z*direction.z)))));*/
		// A = Sqrt (rot.x^2 + rot.y^2 + rot.z^2)
		// B = Sqrt (dir.x^2 + dir.y^2 + dir.z^2)
		// angle = cos^-1((rot.x*dir.x + rot.y*dir.y + rot.z*dir.z)/A*B
}

void Transform::ApplyFriction(float coeff)
{
	vec3 frict = -velocity;

	normalize(frict);

	frict = frict * coeff;

	acceleration += frict;

}

vec3 Transform::Wandering()
{
	// Wandering
	//vec3 CircleCenter = vec3(position.x, 0, position.z) + vec3(direction.x, 0, direction.z) * jitter;

	//float x = cos(wanderAngle);
	//float z = sin(wanderAngle);
	//vec3 displacement = vec3(x, position.y, z);
	//vec3 seekPoint = CircleCenter + displacement;
	//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//wanderAngle += (r * 360 - (360 * .5));

	//// Seeking
	//vec3 desiredVelocity = seekPoint - position;

	//vec3 steeringForce = desiredVelocity - velocity;

	//return vec3(steeringForce);
	return -vec3(0, 0, 2.5);
}

void Transform::CameraFollow(float dt)
{
	rotMat = (mat3)glm::yawPitchRoll(rotation.x, rotation.y, rotation.z);
	//rotationHold = rotation;
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A))
	{
		ApplyRotate(.5, 0, .393, dt);
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D))
	{
		ApplyRotate(-.5, 0, -.393, dt);
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W))
	{
		ApplyForce(vec3(0, 1, 0));
		ApplyRotate(0, -.393, 0, dt);
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S))
	{
		ApplyForce(vec3(0, -1, 0));
		ApplyRotate(0, .393, 0, dt);
	}
	else
	{
		rotation = vec3(rotation.x, 0, 0);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R))
	{
		ApplyForce(rotMat * vec3(-1, 0, 0));
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F))
	{
		ApplyForce(rotMat * vec3(1, 0, 0));
	}


}

bool Transform::MissileDecay(float dt, float timeTillDeath)
{
	destroyTimer += dt;
	if (destroyTimer >= timeTillDeath)
	{
		destroyTimer = 0;
		return true;
	}
	else return false;
}
