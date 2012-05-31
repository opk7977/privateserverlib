#pragma once

#include "NL_Base.h"
#include "NL_Resource.h"
#include "NL_TemplatePool.h"

namespace FMOD
{

class System;
class Sound;
class Channel;
class NSoundMgr;
class NSound : public NResource
{
	DECL_FRIEND(NSoundMgr)

	//========================
	//	Private Data
	//========================
	static System*	m_system;
	
	Sound*		m_snd;
	Channel*	m_ch;

	int		m_index;
	int		m_groupRef;
	bool	m_bUnique;
	bool	m_bLoop;
	bool	m_bPlay;
	bool	m_bPause;
	bool	m_bMute;
	float	m_volume;
	float	m_pan;

public:
	NSound(void);
	~NSound(void);

	void Init();
	bool Load(const TCHAR* fileName);
	void Remove();
	bool Reset();
	void Release();
	bool IsLoaded() const	{	return !!m_snd;		}

//////////////////////////////////////////////////////////////////////////

	static void Update();
	void Play();
	void Stop();
	void Pause();
	void Mute(bool bMute);
	void Loop(bool bLoop);
	void Volume(float vol);
	void Pan(float pan);
	void DeltaVolume(float delta);
	void DeltaPan(float delta);
	float GetVolume()		{	return m_volume;	}
	float GetPan()			{	return m_pan;		}
	bool IsPlaying();
};

class ChannelGroup;
struct NSoundGroup
{
	CString			name;
	int				sndCount;
	bool			bMute;
	float			volume;
	ChannelGroup*	group;

	NSoundGroup():
		sndCount(0), bMute(false), volume(1.0f), group(NULL)
	{

	}
};

//=========================================================================
//	Name   : NSoundMgr
//	Content: 사운드 매니져
//=========================================================================
class NSoundMgr : public NTemplatePool<NSound>
{
	DECL_FRIEND(NSound)

	//==================================
	//	Private Data
	//==================================
	System*								m_system;
	NSoundGroup							m_globalGroup;
	std::vector<NSoundGroup>			m_subGroup;
	std::vector<NSound>					m_soundVec;
	std::map<int, NSound*>				m_soundMap;
	std::map<int, NSound*>::iterator	m_soundMapItor;
	FILE*								m_fp;
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NSoundMgr();
	~NSoundMgr();

	//==================================
	//	External Function
	//==================================
	bool  Init(int maxChannel = 100);
	bool  LoadList(const TCHAR* fileName);
	bool  PreLoadList(const TCHAR* fileName);
	float SetGroupVolume(int group, float volume);
	float DeltaGroupVolume(int group, float delta);
	float GetGroupVolume(int group);
	void  SetGroupMute(int group, bool bMute);
	bool  GetGroupMute(int group);
	int   GetGroupCount();
	int   GetGroupSoundCount(int group);
	const TCHAR* GetGroupName(int group);
	int	  GetSoundCount();
	const TCHAR* GetSoundNameByNumber(int number);
	const TCHAR* GetSoundName(int index);
	float SetGlobalVolume(float volume);
	float DeltaGlobalVoume(float delta);
	float GetGlobalVolume();
	void  SetGlobalMute(bool bMute);
	bool  GetGlobalMute();
	bool  ResetList(std::vector<int>& resetList);
	void  RemoveList(std::vector<int>& removeList);
	void  Release();

	//=====================================
	//  Sound Manager Function
	//=====================================
	void  Play(int index);
	void  Stop(int index);
	void  Pause(int index);
	void  Mute(int index, bool bMute);
	void  Loop(int index, bool bLoop);
	void  Volume(int index, float vol);
	void  Pan(int index, float pan);
	void  DeltaVolume(int index, float delta);
	void  DeltaPan(int index, float delta);
	float GetVolume(int index);
	float GetVolumeByNumber(int number);

private:
	//==================================
	//	Internal Function
	//==================================
	void ReadNext(TCHAR* buf);
};

};

using namespace FMOD;

#define   Mgr_NSound	 NSoundMgr::GetInstance_T<NSoundMgr>()