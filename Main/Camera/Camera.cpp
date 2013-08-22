#include "Camera.h"


Camera::Camera() : rotationX(0), rotationY(0), locked(false), aspectRatio(1)
{
}


Camera::~Camera()
{
}


void Camera::init()
{
	nearPlaneDistance = 0.1f;
	farPlaneDistance = 20000.0f;
	fieldOfView = 45;

	mouseSpeed = 0.005f;
	mouseWheelSpeed = 0.1f;
	movementSpeed = 50;
	rotationSpeed = 1;

	moveFast = 10;
	moveSlow = 0.1f;

	buttonMoveForward = 'W';
	buttonMoveBackward = 'S';
	buttonMoveLeft = 'A';
	buttonMoveRight = 'D';
	buttonMoveUp = 'Q';
	buttonMoveDown = 'E';

	buttonMoveFast = VK_CONTROL;
	buttonMoveSlow = VK_SHIFT;

	buttonLookDown = VK_DOWN;
	buttonLookUp = VK_UP;
	buttonLookLeft = VK_LEFT;
	buttonLookRight = VK_RIGHT;
}


void Camera::release()
{
}


void Camera::update(const GameTime& gameTime)
{
	Vector2 mouseMovement = Mouse::getMovement();

	yaw(mouseMovement.x * mouseSpeed);
	pitch(mouseMovement.y * mouseSpeed);

	Matrix m = Matrix::createFromAxisAngle(Vector3::up, rotationX);

	Vector3 forward = Vector3::transform(Vector3::forward, m);
	Vector3 right = Vector3::transform(Vector3::right, m);

	Vector3 direction;

	if (Keyboard::isKeyDown(buttonMoveForward))
		direction += forward;

	if (Keyboard::isKeyDown(buttonMoveBackward))
		direction -= forward;

	if (Keyboard::isKeyDown(buttonMoveRight))
		direction += right;

	if (Keyboard::isKeyDown(buttonMoveLeft))
		direction -= right;

	if (Keyboard::isKeyDown(buttonMoveUp))
		direction += Vector3::up;

	if (Keyboard::isKeyDown(buttonMoveDown))
		direction -= Vector3::up;

	float speed = rotationSpeed * gameTime.elapsed;

	//if (Keyboard::isKeyDown(buttonLookDown))
	//	pitch(speed);

	//if (Keyboard::isKeyDown(buttonLookUp))
	//	pitch(-speed);

	//if (Keyboard::isKeyDown(buttonLookRight))
	//	yaw(speed);

	//if (Keyboard::isKeyDown(buttonLookLeft))
	//	yaw(-speed);

	float modifier = 1.0f;

	if (Keyboard::isKeyDown(buttonMoveFast))
		modifier *= moveFast;

	if (Keyboard::isKeyDown(buttonMoveSlow))
		modifier *= moveSlow;

	position += Vector3::normalize(direction) * movementSpeed * modifier * gameTime.elapsed;

	updateRotation();
}


void Camera::updateRotation()
{
  	Quaternion rotX = Quaternion::createFromAxisAngle(Vector3::up, rotationX);
	Quaternion rotY = Quaternion::createFromAxisAngle(Vector3::right, rotationY);

	rotation = rotY * rotX;
}


Matrix Camera::getView() const
{
	return getView(position, rotation);
}


Matrix Camera::getLockedView() const
{
	return getView(lockedPosition, lockedRotation);
}


Matrix Camera::getView(const Vector3& position, const Quaternion& rotation) const
{
	return Matrix::createTranslation(-position) * Matrix::invert(Matrix::createFromQuaternion(rotation));
}


Matrix Camera::getProjection(float nearPlaneDistance, float farPlaneDistance) const
{
	if (!nearPlaneDistance)
		nearPlaneDistance = this->nearPlaneDistance;

	if (!farPlaneDistance)
		farPlaneDistance = this->farPlaneDistance;

	return Matrix::createPerspectiveFieldOfView(D3DXToRadian(fieldOfView), aspectRatio, nearPlaneDistance, farPlaneDistance);
}


const Vector3& Camera::getPosition() const
{ 
	return locked ? lockedPosition : position; 
}


const Quaternion& Camera::getRotation() const 
{ 
	return rotation; 
}


void Camera::toggleLock()
{
	lockedPosition = position;
	lockedRotation = rotation;
	lockedViewFrustum = getViewFrustum();

	locked = !locked;
}


BoundingFrustum Camera::getViewFrustum() const
{
	return (locked) ? lockedViewFrustum : BoundingFrustum(getView() * getProjection());
}


void Camera::pitch(float angle)
{
	rotationY += angle;

	if (std::abs(rotationY) > D3DX_PI / 2.0f)
		rotationY = (D3DX_PI / 2.0f) * ((rotationY > 0) ? 1 : -1);
}


void Camera::yaw(float angle)
{
	rotationX += angle;

	if (std::abs(rotationX) > D3DX_PI * 2)
		rotationX -= (D3DX_PI * 2.0f) * ((rotationX > 0) ? 1 : -1);
}


void Camera::lookAt(const Vector3& target)
{
	look(target - position);
}


void Camera::look(const Vector3& direction)
{
	Vector3 d = Vector3::normalize(direction);
	Vector3 vx = Vector3::normalize(Vector3(d.x, 0, d.z));

	rotationX = std::acos(MathHelper::clamp(Vector3::dot(Vector3::forward, vx), -1.0f, 1.0f));
	rotationY = std::acos(MathHelper::clamp(Vector3::dot(vx, d), -1.0f, 1.0f));

	rotationX *= (direction.x < 0) ? -1 : 1;
	rotationY *= (direction.y < 0) ? 1 : -1;

	updateRotation();
}


Vector3 Camera::getForward() const
{
	return Vector3::transform(Vector3::forward, Matrix::createFromQuaternion(rotation));
}


Vector3 Camera::getUp() const
{
	return Vector3::transform(Vector3::up, Matrix::createFromQuaternion(rotation));
}
