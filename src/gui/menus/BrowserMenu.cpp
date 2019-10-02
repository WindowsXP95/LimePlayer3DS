#include "BrowserMenu.hpp"
#include "gui.hpp"
#include "app.hpp"
#include "config.hpp"
#include "plsparse.hpp"
#include "m3uparse.hpp"
#include "error.hpp"

#include "QuickSetOverlay.hpp"

Thread thread = NULL;

static uint32_t window = 1;
static int cursor = 1;
static int seloffs = 0;

static void exitPlayback(void)
{
	PlayerInterface::ExitPlayback();

	threadJoin(thread, U64_MAX);
	threadFree(thread);
	thread = NULL;
}

static int changeFile(const std::string &filepath, playbackInfo_t* playbackInfo)
{
	s32 prio;

	/**
	* If music is playing, stop it. Only one playback thread should be playing
	* at any time.
	*/
	if(thread != NULL)
		exitPlayback();

	if(!playbackInfo)
		return 0;

	playbackInfo->usePlaylist = 0;

	std::string extension = filepath.substr(filepath.find_last_of('.') + 1);
	if (!strncmp(extension.c_str(), "json", 4)) {
		std::string url;
		Cfg::ParseNC(filepath.c_str(), &url);
		playbackInfo->filepath = url;
	} else if (!strncmp(extension.c_str(), "pls", 3)) {
		// Note to future me.
		// Add pls support to netfmt.
		Pls::Parse(filepath, &playbackInfo->playlistfile);
		playbackInfo->usePlaylist = 1;
	} else if (!strncmp(extension.c_str(), "m3u", 3)) {
		M3u::Parse(filepath, &playbackInfo->playlistfile);
		playbackInfo->usePlaylist = 1;
	} else
		playbackInfo->filepath = filepath;

	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	thread = threadCreate(PlayerInterface::ThreadMainFunct, (void *)playbackInfo, 32 * 1024, prio - 1, -2, false);

	return 0;
}

void List(const char* text, int row)
{
	Gui::PrintColor(text, 8.0f, row*12, 0.4f, 0.4f, 0xFF000000);
}

void BrowserMenu::fblist(int rows, int startpoint, float size) const
{
	for (int i = 0; rows-seloffs > i && i <= MAX_LIST; i++) {
		if (seloffs+i < expInst->Size()) {
			DirEntry drent = expInst->GetEntry(i + seloffs);
			Gui::PrintColor(drent.filename.c_str(), 8.0f, i*size+startpoint, 0.4f, 0.4f, 0xFF000000);
		}
	}
}

void menuList(int cur, int from, float startpoint, float size, int rows)
{
	int i = 0;
	bool color = true;

	if (rows < 1)
		return;

	for (i = 0; rows-from > i && i < MAX_LIST; i++) {
		if (cur-from == i)
			C2D_DrawRectSolid(0, i*size+startpoint, 0.5f, SCREEN_WIDTH, size, C2D_Color32(255, 255, 2, 255));
		else if (color)
			C2D_DrawRectSolid(0, i*size+startpoint, 0.5f, SCREEN_WIDTH, size, C2D_Color32(23, 100, 64, 255));
		else if (!color)
			C2D_DrawRectSolid(0, i*size+startpoint, 0.5f, SCREEN_WIDTH, size, C2D_Color32(43, 191, 63, 255));

		color = !color;
	}
}

void drawBrowserPlayer(playbackInfo_t* info)
{
	Gui::DrawImage(sprites_player_playlist_idx, 20, 15);
	if (!PlayerInterface::GetDecoderName().empty())
		Gui::Print(PlayerInterface::GetDecoderName(), 150.0f, 80.0f, 0.5f, 0.5f);

	if (!info->filename.empty())
		Gui::Print(info->filename, 150.0f, 20.0f, 0.5f, 0.5f);
	else
		Gui::PrintStatic("TEXT_LOADING_GENERIC", 150.0f, 20.0f, 0.5f, 0.5f);

	if (!info->fileMeta.Artist.empty())
		Gui::Print(info->fileMeta.Artist, 150.0f, 40.0f, 0.5f, 0.5f);
	else
		Gui::PrintStatic("TEXT_LOADING_GENERIC", 150.0f, 40.0f, 0.5f, 0.5f);
}

BrowserMenu::BrowserMenu() {
	expInst = std::make_unique<Explorer>("sdmc:/");
	expInst->ChangeDir("music");
}

BrowserMenu::~BrowserMenu()
{
	exitPlayback();
}

void BrowserMenu::drawTop() const
{
	if (PlayerInterface::IsPlaying())
		drawBrowserPlayer(&App::pInfo);

	Gui::DrawBaseGui();
}

void BrowserMenu::drawBottom() const
{
	menuList(cursor, seloffs, 15, 15, expInst->Size());
	C2D_DrawRectSolid(0, 0, 0.5f, SCREEN_WIDTH, 15, C2D_Color32(119, 131, 147, 255));
	List(expInst->GetCurrentDir().c_str(), 0);
	BrowserMenu::fblist(expInst->Size(), 15, 15);
}

void BrowserMenu::update(touchPosition* touch)
{
	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	static u64	mill = 0;

	if (Error::IsQuered()) {
		if (kDown & KEY_A) {
			Error::Remove();
			kDown = 0;
		}
	}
	else {
		if (kHeld & KEY_UP) {
			if (kDown & KEY_RIGHT)
				window++;

			if (kDown & KEY_LEFT)
				window--;
		}

		if (window == 1) {
			if (kDown & KEY_SELECT) {
				addOverlay<QuickSetOverlay>();
			}

			if (kHeld & KEY_L) {
				if(kDown & (KEY_R | KEY_UP)) {
					if(PlayerInterface::IsPlaying())
						PlayerInterface::TogglePlayback();
				}

				if(kDown & KEY_B) {
					exitPlayback();
				}

				if(kDown & KEY_X) {
					PlayerInterface::SkipPlayback();
				}

				return;
			}

			if (kDown & KEY_A) {
				if (expInst->IsDir(cursor)) {
					expInst->ChangeTo(cursor);
					cursor = 1;
					seloffs = 0;
				} else {
					changeFile(expInst->GetAbsolutePath(cursor), &App::pInfo);
				}
			}

			if (kDown & KEY_X) {
				// KUSC http://16643.live.streamtheworld.com/KUSCMP128.mp3
				// RadioSega http://content.radiosega.net:8006/rs-mpeg.mp3
				// RadioNintendo http://play.radionintendo.com/stream
				changeFile("http://play.radionintendo.com/stream", &App::pInfo);
			}

			if (kDown & KEY_B) {
				cursor = 1;
				seloffs = 0;
				expInst->BackDir();
			}

			if((kDown & KEY_UP || ((kHeld & KEY_UP) && (osGetTime() - mill > 500))) && cursor > 0)
			{
				cursor--;

				/* 26 is the maximum number of entries that can be printed */
				if(expInst->Size() - cursor > MAX_LIST && seloffs != 0)
					seloffs--;
			}

			if((kDown & KEY_DOWN || ((kHeld & KEY_DOWN) && (osGetTime() - mill > 500))) && cursor < expInst->Size()-1)
			{
				cursor++;

				if(cursor >= MAX_LIST && expInst->Size() - cursor >= 0 && seloffs < expInst->Size() - MAX_LIST)
					seloffs++;
			}

			if((kDown & KEY_LEFT || ((kHeld & KEY_LEFT) && (osGetTime() - mill > 500))) && cursor > 0)
			{
				int skip = MAX_LIST / 2;

				if(cursor < skip)
					skip = cursor;

				cursor -= skip;

				if(expInst->Size() - cursor > MAX_LIST && seloffs != 0)
				{
					seloffs -= skip;
					if(seloffs < 0)
						seloffs = 0;
				}
			}

			if((kDown & KEY_RIGHT || ((kHeld & KEY_RIGHT) && (osGetTime() - mill > 500))) && cursor < expInst->Size()-1)
			{
				int skip = expInst->Size()-1 - cursor;

				if(skip > MAX_LIST / 2)
					skip = MAX_LIST / 2;

				cursor += skip;

				if(cursor >= MAX_LIST && expInst->Size() - cursor >= 0 && seloffs < expInst->Size() - MAX_LIST)
				{
					seloffs += skip;
					if(seloffs > expInst->Size() - MAX_LIST)
						seloffs = expInst->Size() - MAX_LIST;
				}
			}

			if(kDown) {
				mill = osGetTime();
			}
		}
	}
}