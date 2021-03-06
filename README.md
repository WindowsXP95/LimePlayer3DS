# LimePlayer3DS
LimePlayer3DS is a graphical music player for the 3ds with metadata parsing and software midi support.

It checks for music in the `sdmc:/music` folder. If it doesn't exist then it just goes to `sdmc:/`, just like ctrmus. 

The default path for midi patchsets is `/3ds/limeplayer3ds/dgguspat`.

This path be changed in the config file located at `/3ds/limeplayer3ds/config.json` which should be created (if it doesn't already exist) upon launching the application.

More info about LimePlayer3DS's config.json can be found here: <br/>
https://github.com/Oreo639/LimePlayer3DS/wiki/config.json

[For more information on usage of the various features LimePlayer offers, check here.](https://github.com/Oreo639/LimePlayer3DS/wiki)

TODO:
(GUI redesign and rewrite. Loading cover pictures. Reading of metadata.)

## Note
LimePlayer3DS is currently in beta so not all features are going to be avalible.

## Officaly Supported Formats
Mp3, Wav, Flac, Midi, Xmi, Mus, Hmi, Hmp, Ogg, and Opus.

## Building
### Prerequsites:
[dkp-pacman](https://devkitpro.org/wiki/devkitPro_pacman)

3ds-dev 3ds-mpg123 3ds-libvorbisidec 3ds-opusfile 3ds-flac 3ds-wildmidi 3ds-jansson 3ds-pkg-config

You may also need [makerom](https://github.com/profi200/Project_CTR) and [bannertool](https://github.com/Steveice10/bannertool/) for cia building (optional).

### Compiling:
Once you complete Prerequsites, run `make` or `make <target>`. Replace `<target>` with one of the targets listed below

### Targets:
+ 3dsx (3dsx only build)
+ cia (generate cia, 3dsx must be built first)
+ clean (clean compiled files)

If you want to build with compiler optimazitions, just add RELEASE=1 to the end (EX: `make 3dsx RELEASE=1`)

## Contributors
Check [CONTRIBUTORS.md](CONTRIBUTORS.md) for more information

## Credits
+ LimePlayer started as a fork of ctrmus, meaning that the majority of its code came from, is based off, or was built on top of ctrmus.

+ Deltabeard: For his work on ctrmus which was the original foundation for this application.

+ Astronautlevel & LiquidFernir (and the Anemone3DS team): For their work on Anemone3DS which helped quite a bit when it came to starting with citro2d graphics, and they were very helpful in general.

+ Flagbrew: Some of the code for gui was adapted to work with LimePlayer3DS.

+ Others: If I forgot to mention someone important (as in their code is used without credit), please let me know.

+ Most of the icons under gfx are from the site [icons8.com](https://icons8.com) and are licensed under the [CC-BY-NC-SA](https://creativecommons.org/licenses/by-nc-sa/3.0/)

## Special Thanks
+ Smealum, WinterMute, fincs, and the devkitPro team: For making/maintaining documentation and libraries for libctru and citro2d/3d which were very helpfull when making this project.

+ The Easyrpg Team: For inspiring this application with their work porting wildmidi to the 3ds and the switch (the reason I started working on this was because I wondered if there was a midi player for the 3ds).

+ Family: For providing me helpfull feedback, and testing pre-release versions.

## Copyright
The application is licensed under GPLv3-or-later. Additional terms 7c apply.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
