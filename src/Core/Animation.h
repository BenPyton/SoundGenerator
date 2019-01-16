/*
* @author Benoit PELLETIER
*
* @file Animation.h
*
* @date 5/11/2017
*
* @brief Functions to animate a single double value
*
*/

#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED


//struct Animation
//{
//	double min, max, value;
//	float currentTime, time, delay, currentDelay;
//	bool play, reverse, loop, ended, mirror;
//
//};
//
//
//
//Animation AnimationCreate(float min, float max, float duration, float delay = 0);
//
//void AnimationUpdate(Animation &a, float deltaTime);
//void AnimationPlay(Animation &a);
//void AnimationStop(Animation &a);
//void AnimationReverse(Animation &a, bool reverse);
//void AnimationLoop(Animation &a, bool loop);
//void AnimationMirror(Animation &a, bool mirror);
//bool AnimationIsEnded(Animation &a);

float Lerp(float min, float max, float t);


class Animation
{
private:
	double m_min, m_max, m_value;
	float m_currentTime, m_time, m_delay, m_currentDelay;
	bool m_play, m_reverse, m_loop, m_ended, m_mirror;

public:
	Animation(float min, float max, float duration, float delay = 0);

	void SetReverse(bool reverse);
	void SetLoop(bool loop);
	void SetMirror(bool mirror);
	bool GetReverse() { return m_reverse; }
	bool GetLoop() { return m_loop; }
	bool GetMirror() { return m_mirror; }
	bool IsEnded() { return m_ended; }

	void Update(float deltaTime);
	void Play();
	void Stop();
};


#endif // ANIMATION_H_INCLUDED