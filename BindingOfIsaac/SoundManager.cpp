#include "pch.h"
#include "SoundManager.h"
#include "SoundStream.h"

SoundManager::SoundManager()
	: m_Index{ rand() % 12 }
	, m_Stop{ false }
{
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Rupert Holmes - Escape (The Pina Colada Song).mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Marvin Gaye & Tammi Terrell - Ain't No Mountain High Enough.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Electric Light Orchestra - Mr Blue Sky.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Come And Get Your Love.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Come A Little Bit Closer - Jay & The Americans.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Bobby McFerrin - Don't Worry Be Happy.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Pumped Up Kicks.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Chuck Berry - You Never Can Tell.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/I'm Still Standing.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Katrina & The Waves - Walking On Sunshine.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Matthew Wilder - Break My Stride.mp3" });
	m_pPlaylist.push_back(new SoundStream{ "Resources/Sounds/songs/Queen - I Want to Break Free.mp3" });


	m_pPlaylist[m_Index]->SetVolume(20);
}


SoundManager::~SoundManager()
{
	DeletePlaylist();
}

void SoundManager::Update()
{
	if (m_Stop)
		return;

	if (!m_pPlaylist[m_Index]->IsPlaying())
	{
		m_pPlaylist[m_Index]->Stop();
		++m_Index %= m_pPlaylist.size();
		m_pPlaylist[m_Index]->Play(false);
	}
}

void SoundManager::SetPause()
{
	m_pPlaylist[m_Index]->Pause();
}

void SoundManager::SetResume()
{
	m_pPlaylist[m_Index]->Resume();
	m_Stop = false;
}

void SoundManager::SetStop()
{
	m_pPlaylist[m_Index]->Stop();
	m_Stop = true;
}

void SoundManager::SetVolume(int value)
{
	m_pPlaylist[m_Index]->SetVolume(value);
	std::cout << m_pPlaylist[m_Index]->GetVolume() << '\n';
}

int SoundManager::GetVolume() const
{
	return m_pPlaylist[m_Index]->GetVolume();
}

void SoundManager::DeletePlaylist()
{
	for (SoundStream* pSong : m_pPlaylist)
	{
		delete pSong;
		pSong = nullptr;
	}
	m_pPlaylist.clear();
}
