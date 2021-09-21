#pragma once

#pragma comment(lib, "xaudio2.lib")

#include <xaudio2.h>
#include <x3daudio.h>
#include <vector>
#include <map>
#include <cstddef>
#include <filesystem>
#include <mutex>
#include <type_traits>

#include "Channel.h"

class Audio_System {

public:

	Audio_System() = delete;
	Audio_System(const Audio_System&) = delete;
	void operator=(const Audio_System&) = delete;

	~Audio_System();

	/*
	 * Add a new sub-mixer.
	 *
	 * @param name of the new sub-mixer.
	 * @return bool : if successfully added.
	 */
	static bool addSubMixer(std::string);

	/*
	 * Remove a sub-mixer.
	 *
	 * @param name of the sub-mixer to remove.
	 * @return bool : if successfully removed.
	 */
	static bool removeSubMixer(std::string);



	/*
	 * Set the volume of the master Mixer.
	 *
	 * @param value between 0 and 100.
	 */
	static void setMaster_Volume(float);

	/*
	 * Get the volume of the master Mixer.
	 *
	 * @return value between 0 and 100.
	 */
	static const float getMaster_Volume();



	/*
	 * Set the volume of a sub-mixer.
	 *
	 * @param name of the sub-mixer.
	 * @param value between 0 and 100.
	 */
	static void setSubMixer_Volume(std::string, float);

	/*
	 * Get the volume of a sub-mixer.
	 *
	 * @param name of the sub-mixer.
	 * @return value between 0 and 100.
	 */
	static const float getSubMixer_Volume(std::string);



	/*
	 * Set 3D position of the listener.
	 *
	 * @param x positon.
	 * @param y positon.
	 * @param z positon.
	 */
	static void setListenerPosition(float x, float y, float z);

	/*
	 * Set 3D position of the listener.
	 *
	 * @param x, y and z vector.
	 */
	static void setListenerPosition(X3DAUDIO_VECTOR pos);



	/*
	 * Move 3D position of the listener.
	 *
	 * @param x positon.
	 * @param y positon.
	 * @param z positon.
	 */
	static void moveListenerPosition(float x, float y, float z);

	/*
	 * Move 3D position of the listener.
	 *
	 * @param x, y and z vector.
	 */
	static void moveListenerPosition(X3DAUDIO_VECTOR pos);



	/*
	 * Set the listener Forward(Orientation).
	 *
	 * @param x positon.
	 * @param y positon.
	 * @param z positon.
	 */
	static void setListenerForward(float x, float y, float z);

	/*
	 * Set the listener Forward(Orientation).
	 *
	* @param x, y and z vector.
	 */
	static void setListenerForward(X3DAUDIO_VECTOR forward);



	/*
	 * Set the listener Orientation.
	 *
	 * @param x, y and z vector (Forward).
	 * @param x, y and z vector (direction of top of the head).
	 */
	static void setListenerOrientation(X3DAUDIO_VECTOR forward, X3DAUDIO_VECTOR TopOfHead);

	/*
	 * Set the listener Orientation.
	 *
	 * @param x positon (Forward).
	 * @param y positon (Forward).
	 * @param z positon (Forward).
	 * @param x, y and z vector (direction of top of the head).
	 */
	static void setListenerOrientation(float f_x, float f_y, float f_z, X3DAUDIO_VECTOR TopOfHead);

	/*
	 * Set the listener Orientation.
	 *
	 * @param x positon (Forward).
	 * @param y positon (Forward).
	 * @param z positon (Forward).
	 * @param x positon (direction of top of the head).
	 * @param y positon (direction of top of the head).
	 * @param z positon (direction of top of the head).
	 */
	static void setListenerOrientation(X3DAUDIO_VECTOR forward, float t_x, float t_y, float t_z);

	/*
	 * Set the listener Orientation.
	 *
	 * @param x, y and z vector (Forward).
	 * @param x positon (direction of top of the head).
	 * @param y positon (direction of top of the head).
	 * @param z positon (direction of top of the head).
	 */
	static void setListenerOrientation(float f_x, float f_y, float f_z, float t_x, float t_y, float t_z);



	/*
	 * Give the listener position, x, y, z.
	 *
	 * @param T : template with contructor float, float, float.
	 * @param Default = Vector3 (Float).
	 *
	 * @return T : position.
	 */
	template <typename T = X3DAUDIO_VECTOR>
	static const T getListenerPosition() {
		if constexpr (std::is_same_v<T, decltype(m_X3DListener.Position)>) {
			return m_X3DListener.Position;
		}
		else {
			static_assert(std::is_constructible_v<T, float, float, float>, "getListenerPosition() template invalid.");

			return T(m_X3DListener.Position.x, m_X3DListener.Position.y, m_X3DListener.Position.z);
		}
	}

	/*
	 * Give the listener Forward(Orientation), x, y, z.
	 *
	 * @param T : template with contructor float, float, float.
	 * @param Default = Vector3 (Float).
	 *
	 * @return T : Forward(Orientation).
	 */
	template <typename T = X3DAUDIO_VECTOR>
	static const T getListenerForwad() {
		if constexpr (std::is_same_v<T, decltype(m_X3DListener.OrientFront)>) {
			return m_X3DListener.OrientFront;
		}
		else {
			static_assert(std::is_constructible_v<T, float, float, float>, "getListenerPosition() template invalid.");

			return T(m_X3DListener.OrientFront.x, m_X3DListener.OrientFront.y, m_X3DListener.OrientFront.z);
		}
	}



	/*
	 * Set the listener Max hearing distance.
	 *
	 * @param new listener Max hearing distance.
	 */
	static void setDefaultMaxDistance(float maxDistance);

	/*
	 * Give the listener Max hearing distance.
	 *
	 * @return actual listener Max hearing distance.
	 */
	static const float getDefaultMaxDistance();

private:

	static void recalculate3D();
	static void removeChannel_(class Channel& channel);


	friend struct constructor;
	struct constructor {
		constructor();
		~constructor() = default;
	};

	inline static int m_MaxChannelsPerMixer{ 64 };
	inline static IXAudio2* m_pEngine{ nullptr }; // thread etc... ?
	inline static IXAudio2MasteringVoice* m_pMaster{ nullptr }; // need only for engine ? can't interact directly
	inline static std::map<std::string, IXAudio2SubmixVoice*> m_SubVoices;

	inline static X3DAUDIO_HANDLE m_X3DInstance;
	inline static X3DAUDIO_LISTENER m_X3DListener;
	inline static float m_DefaultMaxDistance{ 100 };

	inline static std::mutex m_ChannelList_mutex;
	inline static std::map<std::string, std::vector<class std::unique_ptr<class Channel>>> m_activeChannelPtrs;

	inline static constructor m_constructor; // constructor called automatically on lauching.

	inline static std::map<std::filesystem::path, std::shared_ptr<class FileProcessing>> m_buffers;

#ifdef _AUDIO_ADMIN_
public:

#pragma region internal_use_admin

	/*
	 * Get path and FileProcessing.
	 *
	 * @return map with path and FileProcessing.
	 */
	static std::map<std::filesystem::path, std::shared_ptr<class FileProcessing>>& getBuffer() {
		return m_buffers;
	}



	/*
	 * Get path and FileProcessing.
	 *
	 * @return map with path and FileProcessing.
	 */
	static std::map<std::string, IXAudio2SubmixVoice*>& getSubVoices() {
		return m_SubVoices;
	}



	/*
	 * Get IXAudio2 engine.
	 *
	 * @return IXAudio2 pointer.
	 */
	static IXAudio2* getEngine() {
		return m_pEngine;
	}



	/*
	 * Get Master voice.
	 *
	 * @return IXAudio2MasteringVoice pointer.
	 */
	static IXAudio2MasteringVoice* getMasterVoice() {
		return m_pMaster;
	}



	/*
	 * Get X3DAUDIO Handle.
	 *
	 * @return X3DAUDIO_HANDLE.
	 */
	static X3DAUDIO_HANDLE& getX3DAUDIO_Handle() {
		return m_X3DInstance;
	}



	/*
	 * Get Listener struct.
	 *
	 * @return X3DAUDIO_LISTENER.
	 */
	static X3DAUDIO_LISTENER& getListener() {
		return m_X3DListener;
	}



	/*
	 * Remove a channel.
	 *
	 * @param channel to remove.
	 */
	static void removeChannel(class Channel& channel) {
		Audio_System::removeChannel_(channel);
	}



	/*
	 * Get master voice details.
	 *
	 * @return XAUDIO2_VOICE_DETAILS.
	 */
	static XAUDIO2_VOICE_DETAILS getVoiceDetails() {
		XAUDIO2_VOICE_DETAILS deviceDetails;
		Audio_System::m_pMaster->GetVoiceDetails(&deviceDetails);
		return deviceDetails;
	}



	/*
	 erase buffers if unused.
	 */
	static void updateBuffersLife() {
		std::vector<std::filesystem::path> erraseList;
		for (const auto& [path, buffer] : m_buffers) {
			if (buffer.use_count() <= 1) {
				erraseList.push_back(path);
			}
		}

		for (const auto& path : erraseList) {
			m_buffers.erase(path);
		}
	}



	/*
	 * add and play a new channel of a sound.
	 *
	 * @param sound to play.
	 */
	static void play(class Sound& sound);

#pragma endregion internal_use_admin

#pragma region user_admin_access

	/*
	 * Get all actives channels.
	 *
	 * @return map of name of mixer and channel pointer.
	 */
	static std::map<std::string, std::vector<class std::unique_ptr<class Channel>>>& getActiveChannels() {
		return m_activeChannelPtrs;
	}



	/*
	 * Get mutex for getActiveChannels.
	 *
	 * @return std::mutex.
	 */
	static std::mutex& channelList_mutex() {
		return m_ChannelList_mutex;
	}

#pragma endregion user_admin_access

#endif // _AUDIO_ADMIN_
};
