#pragma once

#include "..\Types\Types.h"
#include "..\Input\Input.h"
#include "..\Helpers\Helpers.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void init();
	virtual void release();
	virtual void update(const GameTime& gameTime);

	virtual Matrix getView() const;
	virtual Matrix getProjection(float nearPlaneDistance = 0.0f, float farPlaneDistance = 0.0f) const;

	virtual BoundingFrustum getViewFrustum() const;

	virtual const Vector3& getPosition() const;
	virtual const Quaternion& getRotation() const;
	virtual float getNearPlaneDistance() const { return nearPlaneDistance; }
	virtual float getFarPlaneDistance() const { return farPlaneDistance; }
	virtual float getMovementSpeed() const { return movementSpeed; }

	virtual void setMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; }
	virtual void setPosition(const Vector3& position) { this->position = position; }
	virtual void setNearPlaneDistance(float nearPlaneDistance) { this->nearPlaneDistance = nearPlaneDistance; }
	virtual void setFarPlaneDistance(float farPlaneDistance) { this->farPlaneDistance = farPlaneDistance; }
	virtual void setAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }

	virtual void look(const Vector3& direction);
	virtual void lookAt(const Vector3& target);

	virtual Vector3 getForward() const;
	virtual Vector3 getUp() const;

	Matrix getLockedView() const;

	void toggleLock();
	bool isLocked() const { return locked; }

protected:
	float rotationX;
	float rotationY;

	float nearPlaneDistance;
	float farPlaneDistance;
	float fieldOfView;
	float aspectRatio;

	float mouseSpeed;
	float movementSpeed;
	float rotationSpeed;
	float mouseWheelSpeed;

	float moveFast;
	float moveSlow;

	unsigned buttonMoveForward;
	unsigned buttonMoveBackward;
	unsigned buttonMoveLeft;
	unsigned buttonMoveRight;
	unsigned buttonMoveUp;
	unsigned buttonMoveDown;

	unsigned buttonMoveFast;
	unsigned buttonMoveSlow;

	unsigned buttonLookUp;
	unsigned buttonLookDown;
	unsigned buttonLookLeft;
	unsigned buttonLookRight;

	unsigned buttonShowCursor;

	void pitch(float angle);
	void yaw(float angle);

private:
	void updateRotation();
	Matrix getView(const Vector3& position, const Quaternion& rotation) const;

	Vector3 position;
	Quaternion rotation;

	bool locked;

	Vector3 lockedPosition;
	Quaternion lockedRotation;
	BoundingFrustum lockedViewFrustum;
};