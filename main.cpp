#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

//  FACE SPRITE HELPER
static CCNode* makeFace(const std::string& faceName, const std::string& speaker) {
    std::string filename = faceName + ".png";
    auto* sprite = CCSprite::create(filename.c_str());
    
    if (sprite) {
        sprite->setScale(70.f / sprite->getContentSize().width);
        return sprite;// ═══════════════════════════════════════════════════════════
//  Gravity Falls: The Journal — Geometry Dash Geode Mod
// ═══════════════════════════════════════════════════════════

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

// ═══════════════════════════════════════════════════════════
//  SAVE KEYS
//  Each key is like a checkbox the game remembers forever.
//  0 = not done yet    1 = done
// ═══════════════════════════════════════════════════════════
static const char* KEY_INTRO_DONE            = "gf_intro_done";
static const char* KEY_SHACK_DONE            = "gf_shack_done";
static const char* KEY_GARAGE_HINT_SHOWN     = "gf_garage_hint_shown";
static const char* KEY_IN_CHALLENGE          = "gf_in_challenge";
static const char* KEY_CHALLENGE_DONE        = "gf_challenge_done";
static const char* KEY_BOSS_DONE             = "gf_boss_done";
static const char* KEY_BASEMENT_DONE         = "gf_basement_done";
static const char* KEY_IN_GNOME_LEVEL        = "gf_in_gnome_level";
static const char* KEY_GNOME_LEVEL_DONE      = "gf_gnome_level_done";
static const char* KEY_FIGHT_PREP_SHACK_DONE = "gf_fight_prep_shack_done";
static const char* KEY_ON_GIDEON_ACCOUNT     = "gf_on_gideon_account";

// ── Level IDs ──
// Change these to the actual level IDs you want to use!
static constexpr int CHALLENGE_LEVEL_ID  = 128;   // Monster's challenge (Act 1)
static constexpr int GNOME_LEVEL_ID      = 129;   // Jeff's gnome training level (Act 2)

// Gideon's GD account name — player must visit this profile
static const char* GIDEON_ACCOUNT_NAME = "GideonGleeful";

// ═══════════════════════════════════════════════════════════
//  DIALOGUE LINE
//  One line of dialogue: who says it, what, which face,
//  and optionally choice buttons like YES / NO
// ═══════════════════════════════════════════════════════════
struct DialogueLine {
    std::string speaker;
    std::string text;
    std::string face;
    std::vector<std::string> buttons;
};

// ═══════════════════════════════════════════════════════════
//  TEXT HELPER — replaces %username with the player's GD name
// ═══════════════════════════════════════════════════════════
static std::string fillText(const std::string& text) {
    std::string username = GameManager::get()->m_playerName;
    std::string result = text;
    std::string placeholder = "%username";
    size_t pos = result.find(placeholder);
    while (pos != std::string::npos) {
        result.replace(pos, placeholder.length(), username);
        pos = result.find(placeholder, pos + username.length());
    }
    return result;
}

// ═══════════════════════════════════════════════════════════
//  JSON LOADER — finds a scene by id and returns its lines
// ═══════════════════════════════════════════════════════════
static std::vector<DialogueLine> loadScene(const std::string& sceneId) {
    std::vector<DialogueLine> result;
    auto jsonPath = Mod::get()->getResourcesDir() / "dialogue" / "act1.json";
    auto jsonRes = file::readString(jsonPath);
    if (!jsonRes) return result;
    auto parsed = matjson::parse(jsonRes.unwrap());
    if (!parsed) return result;
    auto root = parsed.unwrap();
    if (!root.contains("scenes")) return result;

    for (auto& scene : root["scenes"].asArray().unwrap()) {
        if (scene["id"].asString().unwrap() != sceneId) continue;
        for (auto& line : scene["lines"].asArray().unwrap()) {
            DialogueLine dl;
            dl.speaker = line["speaker"].asString().unwrap_or("???");
            dl.text    = fillText(line["text"].asString().unwrap_or(""));
            dl.face    = line["face"].asString().unwrap_or("");
            if (line.contains("buttons")) {
                for (auto& btn : line["buttons"].asArray().unwrap())
                    dl.buttons.push_back(btn.asString().unwrap_or("OK"));
            }
            result.push_back(dl);
        }
        break;
    }
    return result;
}

// ═══════════════════════════════════════════════════════════
//  FACE HELPER
//  Loads a PNG from resources/faces/
//  Falls back to a colored GD icon if the PNG doesn't exist yet
// ═══════════════════════════════════════════════════════════
static CCNode* makeFace(const std::string& faceName, const std::string& speaker) {
    auto facePath = Mod::get()->getResourcesDir() / "faces" / (faceName + ".png");
    auto* sprite = CCSprite::create(facePath.string().c_str());
    if (sprite) {
        sprite->setScale(70.f / sprite->getContentSize().width);
        return sprite;
    }
    // Fallback colored GD icon
    auto* player = SimplePlayer::create(0);
    if      (speaker == "Dipper")       player->setColors(ccc3(50,  80,  200), ccc3(200, 210, 255));
    else if (speaker == "Mabel")        player->setColors(ccc3(220, 60,  180), ccc3(255, 180, 240));
    else if (speaker == "Soos")         player->setColors(ccc3(180, 110, 40),  ccc3(255, 210, 140));
    else if (speaker == "Grunkle Stan") player->setColors(ccc3(60,  55,  55),  ccc3(160, 140, 90));
    else if (speaker == "Monster")      player->setColors(ccc3(20,  140, 20),  ccc3(80,  255, 80));
    else if (speaker == "Jeff")         player->setColors(ccc3(80,  120, 40),  ccc3(180, 220, 100));
    else if (speaker == "Schmebulock")  player->setColors(ccc3(60,  100, 30),  ccc3(140, 200, 80));
    else                                player->setColors(ccc3(120, 120, 120), ccc3(220, 220, 220));
    player->updatePlayerFrame(1, IconType::Cube);
    player->setScale(0.85f);
    return player;
}

// ═══════════════════════════════════════════════════════════
//  DIALOGUE POPUP
//  The dialogue box that appears on screen.
//  Shows lines one at a time with a typewriter effect.
//  >> button goes to the next line.
//  When all lines are done it calls onDone so the
//  next part of the story can begin.
// ═══════════════════════════════════════════════════════════
class DialoguePopup : public CCLayer {
    std::vector<DialogueLine> m_lines;
    int m_index = 0;
    std::function<void(std::string)> m_onDone;
    CCLabelBMFont* m_textLabel = nullptr;
    std::string m_fullText;
    int m_charIndex = 0;

    void showLine(int idx) {
        this->removeAllChildrenWithCleanup(true);
        this->unschedule(schedule_selector(DialoguePopup::typewriterTick));

        if (idx >= (int)m_lines.size()) {
            if (m_onDone) m_onDone("done");
            this->removeFromParent();
            return;
        }

        auto& line = m_lines[idx];
        auto winSize = CCDirector::get()->getWinSize();
        float cx = winSize.width / 2.f;
        float cy = winSize.height / 2.f;

        // Dark overlay
        auto overlay = CCLayerColor::create({0, 0, 0, 150});
        overlay->setContentSize(winSize);
        this->addChild(overlay, 0);

        // Main panel — bounces in
        auto panel = CCScale9Sprite::create("GJ_square02.png");
        panel->setContentSize({380, 115});
        panel->setPosition({cx, cy - 55});
        panel->setScale(0.5f);
        panel->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3f, 1.0f)));
        this->addChild(panel, 1);

        // Portrait background
        auto portraitBg = CCScale9Sprite::create("GJ_square05.png");
        portraitBg->setContentSize({85, 85});
        portraitBg->setPosition({cx - 152, cy - 55});
        this->addChild(portraitBg, 2);

        // Face image
        auto face = makeFace(line.face, line.speaker);
        face->setPosition({cx - 152, cy - 55});
        this->addChild(face, 3);

        // Speaker name
        auto nameLbl = CCLabelBMFont::create(line.speaker.c_str(), "goldFont.fnt");
        nameLbl->setScale(0.52f);
        nameLbl->setAnchorPoint({0.f, 0.5f});
        nameLbl->setPosition({cx - 102, cy - 22});
        this->addChild(nameLbl, 3);

        // Gold divider line
        auto divider = CCLayerColor::create({255, 200, 50, 180}, 238, 1);
        divider->setPosition({cx - 104, cy - 33});
        this->addChild(divider, 3);

        // Typewriter text
        m_fullText  = line.text;
        m_charIndex = 0;
        m_textLabel = CCLabelBMFont::create("", "chatFont.fnt");
        m_textLabel->setScale(0.48f);
        m_textLabel->setWidth(228);
        m_textLabel->setAlignment(kCCTextAlignmentLeft);
        m_textLabel->setAnchorPoint({0.f, 1.f});
        m_textLabel->setPosition({cx - 102, cy - 38});
        this->addChild(m_textLabel, 3);
        this->schedule(schedule_selector(DialoguePopup::typewriterTick), 0.03f);

        // Buttons
        auto btnMenu = CCMenu::create();
        btnMenu->setPosition({0, 0});
        this->addChild(btnMenu, 4);

        if (line.buttons.size() >= 2) {
            float startX = cx - 40;
            for (auto& label : line.buttons) {
                auto btnSpr = ButtonSprite::create(
                    label.c_str(), "goldFont.fnt",
                    label == "YES" ? "GJ_button_01.png" : "GJ_button_06.png", 0.7f
                );
                auto btn = CCMenuItemSpriteExtra::create(
                    btnSpr, this, menu_selector(DialoguePopup::onChoiceBtn)
                );
                btn->setTag(label == "YES" ? 1 : 0);
                btn->setPosition({startX, cy - 98});
                btnMenu->addChild(btn);
                startX += 85;
            }
        } else {
            auto nextSpr = ButtonSprite::create(">>", "goldFont.fnt", "GJ_button_01.png", 0.65f);
            auto nextBtn = CCMenuItemSpriteExtra::create(
                nextSpr, this, menu_selector(DialoguePopup::onNext)
            );
            nextBtn->setPosition({cx + 150, cy - 98});
            nextBtn->setOpacity(0);
            nextBtn->runAction(CCSequence::create(
                CCDelayTime::create(0.4f), CCFadeIn::create(0.2f), nullptr
            ));
            btnMenu->addChild(nextBtn);
        }
    }

    void typewriterTick(float) {
        if (m_charIndex < (int)m_fullText.length()) {
            m_charIndex++;
            m_textLabel->setString(m_fullText.substr(0, m_charIndex).c_str());
        } else {
            this->unschedule(schedule_selector(DialoguePopup::typewriterTick));
        }
    }

    void onNext(CCObject*) {
        if (m_charIndex < (int)m_fullText.length()) {
            // Show all text at once if player clicks early
            m_charIndex = (int)m_fullText.length();
            m_textLabel->setString(m_fullText.c_str());
            this->unschedule(schedule_selector(DialoguePopup::typewriterTick));
        } else {
            showLine(++m_index);
        }
    }

    void onChoiceBtn(CCObject* sender) {
        auto* btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        std::string chosen = (btn->getTag() == 1) ? "YES" : "NO";
        if (m_onDone) m_onDone(chosen);
        this->removeFromParent();
    }

public:
    static DialoguePopup* create(
        const std::vector<DialogueLine>& lines,
        std::function<void(std::string)> onDone = nullptr
    ) {
        auto* ret = new DialoguePopup();
        if (ret->init()) {
            ret->autorelease();
            ret->m_lines  = lines;
            ret->m_onDone = onDone;
            ret->showLine(0);
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

// ═══════════════════════════════════════════════════════════
//  BOSS LAYER — Act 1 boss fight (the vault monster)
// ═══════════════════════════════════════════════════════════
class BossLayer : public CCLayer {
    int m_phase = 0;

    bool init() override {
        if (!CCLayer::init()) return false;
        auto winSize = CCDirector::get()->getWinSize();
        auto bg = CCLayerColor::create({8, 8, 25, 255});
        this->addChild(bg, 0);
        auto monsterLbl = CCLabelBMFont::create("[ MONSTER ]", "goldFont.fnt");
        monsterLbl->setPosition({winSize.width / 2, winSize.height * 0.72f});
        monsterLbl->setTag(999);
        this->addChild(monsterLbl, 1);
        auto playerLbl = CCLabelBMFont::create("[ YOU ]", "goldFont.fnt");
        playerLbl->setColor({80, 200, 255});
        playerLbl->setPosition({winSize.width / 2, winSize.height * 0.28f});
        this->addChild(playerLbl, 1);
        this->setKeyboardEnabled(true);
        this->scheduleOnce(schedule_selector(BossLayer::showOpening), 0.5f);
        return true;
    }

    // SCENE: boss_fight_start
    // Triggered: when boss fight begins
    void showOpening(float) {
        auto popup = DialoguePopup::create(loadScene("boss_fight_start"));
        this->addChild(popup, 10);
    }

    void keyDown(enumKeyCodes key) override {
        if (key != KEY_E) return;
        if (m_phase == 0) {
            m_phase = 1;
            throwWeapon("~ water bottle ~", 0.5f,
                schedule_selector(BossLayer::afterWaterThrown));
        } else if (m_phase == 1) {
            m_phase = 2;
            throwWeapon("~ screwdriver ~", 0.4f,
                schedule_selector(BossLayer::afterScrewdriverThrown));
        }
    }

    void throwWeapon(const char* label, float speed, SEL_SCHEDULE next) {
        auto winSize = CCDirector::get()->getWinSize();
        auto lbl = CCLabelBMFont::create(label, "chatFont.fnt");
        lbl->setPosition({80, winSize.height / 2});
        this->addChild(lbl, 5);
        lbl->runAction(CCSequence::create(
            CCMoveBy::create(speed, {winSize.width - 120, 0}),
            CCRemoveSelf::create(), nullptr
        ));
        this->scheduleOnce(next, speed + 0.2f);
    }

    // SCENE: water_warning → then soos_saves
    // Triggered: after water bottle is thrown
    void afterWaterThrown(float) {
        auto popup = DialoguePopup::create(loadScene("water_warning"), [this](std::string) {
            // SCENE: soos_saves
            // Triggered: after water_warning finishes
            auto soosPopup = DialoguePopup::create(loadScene("soos_saves"));
            this->addChild(soosPopup, 10);
        });
        this->addChild(popup, 10);
    }

    // Monster dies after screwdriver
    // Triggered: after screwdriver is thrown
    void afterScrewdriverThrown(float) {
        if (auto* m = this->getChildByTag(999)) {
            m->runAction(CCSequence::create(
                CCFadeOut::create(0.6f), CCRemoveSelf::create(), nullptr
            ));
        }
        this->scheduleOnce(schedule_selector(BossLayer::showVictory), 1.0f);
    }

    // SCENE: monster_defeated
    // Triggered: after monster fades out
    void showVictory(float) {
        auto popup = DialoguePopup::create(loadScene("monster_defeated"), [this](std::string) {
            GameManager::get()->setIntValue(KEY_BOSS_DONE, 1);
            // Go to menu — basement button will appear there
            CCDirector::get()->replaceScene(
                CCTransitionFade::create(0.5f, MenuLayer::scene(false))
            );
        });
        this->addChild(popup, 10);
    }

public:
    static CCScene* scene() {
        auto s = CCScene::create();
        auto l = new BossLayer();
        if (l->init()) { l->autorelease(); s->addChild(l); }
        return s;
    }
};

// ═══════════════════════════════════════════════════════════
//  GARAGE LAYER — the secret vault in the garage
// ═══════════════════════════════════════════════════════════
class GarageLayer : public CCLayer {
    bool init() override {
        if (!CCLayer::init()) return false;
        auto winSize = CCDirector::get()->getWinSize();
        auto bg = CCLayerColor::create({18, 12, 8, 255});
        this->addChild(bg, 0);
        auto title = CCLabelBMFont::create("The Garage", "goldFont.fnt");
        title->setScale(0.8f);
        title->setPosition({winSize.width / 2, winSize.height * 0.82f});
        this->addChild(title, 1);
        auto vaultBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("[ VAULT DOOR ]", "goldFont.fnt", "GJ_button_05.png", 0.75f),
            this, menu_selector(GarageLayer::onVaultClick)
        );
        auto menu = CCMenu::create(vaultBtn, nullptr);
        menu->setPosition({winSize.width / 2, winSize.height / 2});
        this->addChild(menu, 2);
        return true;
    }

    // SCENE: garage_vault → monster_appears
    // Triggered: player clicks the vault door
    void onVaultClick(CCObject*) {
        auto popup = DialoguePopup::create(loadScene("garage_vault"), [this](std::string) {
            // SCENE: monster_appears
            // Triggered: after garage_vault finishes
            auto monsterPopup = DialoguePopup::create(
                loadScene("monster_appears"), [this](std::string) {
                    // Send player to the challenge level
                    GameManager::get()->setIntValue(KEY_IN_CHALLENGE, 1);
                    CCDirector::get()->replaceScene(
                        CCTransitionFade::create(0.5f, MenuLayer::scene(false))
                    );
                }
            );
            this->addChild(monsterPopup, 10);
        });
        this->addChild(popup, 10);
    }

public:
    static CCScene* scene() {
        auto s = CCScene::create();
        auto l = new GarageLayer();
        if (l->init()) { l->autorelease(); s->addChild(l); }
        return s;
    }
};

// ═══════════════════════════════════════════════════════════
//  SHACK LAYER — Stan sells things, Mabel and Soos join
// ═══════════════════════════════════════════════════════════
class ShackLayer : public CCLayer {
    bool init() override {
        if (!CCLayer::init()) return false;
        auto winSize = CCDirector::get()->getWinSize();
        auto bg = CCLayerColor::create({55, 30, 15, 255});
        this->addChild(bg, 0);
        auto sign = CCLabelBMFont::create("Mystery Shack", "goldFont.fnt");
        sign->setScale(0.9f);
        sign->setPosition({winSize.width / 2, winSize.height * 0.85f});
        this->addChild(sign, 1);
        this->scheduleOnce(schedule_selector(ShackLayer::startDialogue), 0.3f);
        return true;
    }

    // SCENE: mystery_shack
    // Triggered: when shack layer loads
    void startDialogue(float) {
        auto popup = DialoguePopup::create(loadScene("mystery_shack"), [](std::string) {
            GameManager::get()->setIntValue(KEY_SHACK_DONE, 1);
            CCDirector::get()->replaceScene(
                CCTransitionFade::create(0.5f, MenuLayer::scene(false))
            );
        });
        this->addChild(popup, 10);
    }

public:
    static CCScene* scene() {
        auto s = CCScene::create();
        auto l = new ShackLayer();
        if (l->init()) { l->autorelease(); s->addChild(l); }
        return s;
    }
};

// ═══════════════════════════════════════════════════════════
//  MENU LAYER HOOK
//
//  This runs every time the main menu opens.
//  It checks the save keys and triggers the right scene.
//
//  THE FULL STORY ORDER:
//
//  [intro]
//    → Yes/No choice
//    → [intro_yes] or [intro_no]
//    → Mystery Shack layer
//      → [mystery_shack]
//      → Main menu loads
//        → [main_menu_hint] (once)
//        → "The Garage" button appears
//          → Garage layer
//            → [garage_vault]
//            → [monster_appears]
//            → Player goes to CHALLENGE_LEVEL_ID
//              → Player beats it
//              → [monster_post_level]
//              → Boss fight layer
//                → [boss_fight_start]
//                → [water_warning]
//                → [soos_saves]
//                → [monster_defeated]
//                → Main menu loads
//                  → "The Basement" button appears
//                    → [basement_gnomes]
//                    → Player goes to GNOME_LEVEL_ID
//                      → Player beats it
//                      → [fight_prep_basement]
//                      → Back to main menu
//                        → [fight_prep_shack]
//                        → Mabel tells player to go to GideonGleeful account
//                          → Player visits that profile in GD
//                            → [account_enter_gideon]
//                            → (Act 3 continues here...)
// ═══════════════════════════════════════════════════════════
class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto* gm = GameManager::get();

        // Read all save keys
        bool introDone        = gm->getIntValue(KEY_INTRO_DONE);
        bool shackDone        = gm->getIntValue(KEY_SHACK_DONE);
        bool inChallenge      = gm->getIntValue(KEY_IN_CHALLENGE);
        bool challengeDone    = gm->getIntValue(KEY_CHALLENGE_DONE);
        bool bossDone         = gm->getIntValue(KEY_BOSS_DONE);
        bool basementDone     = gm->getIntValue(KEY_BASEMENT_DONE);
        bool inGnomeLevel     = gm->getIntValue(KEY_IN_GNOME_LEVEL);
        bool gnomeLevelDone   = gm->getIntValue(KEY_GNOME_LEVEL_DONE);
        bool fightPrepDone    = gm->getIntValue(KEY_FIGHT_PREP_SHACK_DONE);
        bool onGideonAccount  = gm->getIntValue(KEY_ON_GIDEON_ACCOUNT);

        // ════════════════════════════════════════════════════
        // STEP 1 — Very first time opening the game
        // SCENE: intro
        // ════════════════════════════════════════════════════
        if (!introDone) {
            this->scheduleOnce([this](float) {
                auto popup = DialoguePopup::create(
                    loadScene("intro"),
                    [this](std::string choice) {
                        // Player chose YES or NO
                        std::string followScene = (choice == "YES") ? "intro_yes" : "intro_no";
                        // SCENE: intro_yes or intro_no
                        // Triggered: immediately after YES/NO is clicked
                        auto followPopup = DialoguePopup::create(
                            loadScene(followScene),
                            [](std::string) {
                                GameManager::get()->setIntValue(KEY_INTRO_DONE, 1);
                                // Go to Mystery Shack
                                CCDirector::get()->replaceScene(
                                    CCTransitionFade::create(0.5f, ShackLayer::scene())
                                );
                            }
                        );
                        CCDirector::get()->getRunningScene()->addChild(followPopup, 200);
                    }
                );
                CCDirector::get()->getRunningScene()->addChild(popup, 200);
            }, 0.6f);

        // ════════════════════════════════════════════════════
        // STEP 2 — Intro done but shack not visited yet
        // Go straight to shack
        // ════════════════════════════════════════════════════
        } else if (!shackDone) {
            this->scheduleOnce([](float) {
                CCDirector::get()->replaceScene(
                    CCTransitionFade::create(0.5f, ShackLayer::scene())
                );
            }, 0.3f);

        // ════════════════════════════════════════════════════
        // STEP 3 — Shack done, boss not done yet
        // Show garage button and hint, watch for level completion
        // ════════════════════════════════════════════════════
        } else if (shackDone && !bossDone) {
            // Always show the garage button
            this->scheduleOnce([this](float) { addGarageButton(); }, 0.2f);

            // SCENE: main_menu_hint
            // Triggered: first time arriving at menu after shack
            if (!gm->getIntValue(KEY_GARAGE_HINT_SHOWN)) {
                this->scheduleOnce([this](float) {
                    auto popup = DialoguePopup::create(loadScene("main_menu_hint"));
                    this->addChild(popup, 200);
                    GameManager::get()->setIntValue(KEY_GARAGE_HINT_SHOWN, 1);
                }, 0.7f);
            }

            // SCENE: monster_post_level
            // Triggered: player just beat the challenge level
            if (inChallenge && !challengeDone) {
                gm->setIntValue(KEY_IN_CHALLENGE, 0);
                gm->setIntValue(KEY_CHALLENGE_DONE, 1);
                this->scheduleOnce([this](float) {
                    auto popup = DialoguePopup::create(
                        loadScene("monster_post_level"),
                        [](std::string) {
                            // After monster reacts → boss fight starts
                            CCDirector::get()->replaceScene(
                                CCTransitionFade::create(0.5f, BossLayer::scene())
                            );
                        }
                    );
                    this->addChild(popup, 200);
                }, 0.5f);
            }

        // ════════════════════════════════════════════════════
        // STEP 4 — Boss done, basement not done yet
        // Show basement button
        // ════════════════════════════════════════════════════
        } else if (bossDone && !basementDone) {
            this->scheduleOnce([this](float) { addBasementButton(); }, 0.2f);

        // ════════════════════════════════════════════════════
        // STEP 5 — Basement visited, gnome level not done yet
        // Player must beat the gnome training level
        // ════════════════════════════════════════════════════
        } else if (basementDone && !gnomeLevelDone) {
            // SCENE: fight_prep_basement
            // Triggered: player just beat the gnome training level
            if (inGnomeLevel && !gnomeLevelDone) {
                gm->setIntValue(KEY_IN_GNOME_LEVEL, 0);
                gm->setIntValue(KEY_GNOME_LEVEL_DONE, 1);
                this->scheduleOnce([this](float) {
                    auto popup = DialoguePopup::create(
                        loadScene("fight_prep_basement"),
                        [this](std::string) {
                            // After basement prep → go to shack for fight prep
                            showFightPrepShack();
                        }
                    );
                    this->addChild(popup, 200);
                }, 0.5f);
            }

        // ════════════════════════════════════════════════════
        // STEP 6 — Gnome level done, fight prep at shack not done
        // SCENE: fight_prep_shack
        // Triggered: first time here after gnome level
        // ════════════════════════════════════════════════════
        } else if (gnomeLevelDone && !fightPrepDone) {
            this->scheduleOnce([this](float) { showFightPrepShack(); }, 0.5f);

        // ════════════════════════════════════════════════════
        // STEP 7 — Fight prep done, player must visit Gideon's account
        // SCENE: account_enter_gideon
        // Triggered: player visits the GideonGleeful profile
        // (detected in ProfilePage hook below)
        // ════════════════════════════════════════════════════
        } else if (fightPrepDone && onGideonAccount) {
            gm->setIntValue(KEY_ON_GIDEON_ACCOUNT, 0);
            this->scheduleOnce([this](float) {
                auto popup = DialoguePopup::create(
                    loadScene("account_enter_gideon"),
                    [](std::string) {
                        // TODO: Act 3 begins here!
                    }
                );
                this->addChild(popup, 200);
            }, 0.5f);
        }

        return true;
    }

    // Shows fight_prep_shack scene then marks it done
    void showFightPrepShack() {
        auto popup = DialoguePopup::create(
            loadScene("fight_prep_shack"),
            [](std::string) {
                GameManager::get()->setIntValue(KEY_FIGHT_PREP_SHACK_DONE, 1);
                // Mabel just told the player to visit GideonGleeful's account
                // The player now has to find and visit that profile in GD
            }
        );
        CCDirector::get()->getRunningScene()->addChild(popup, 200);
    }

    // ── The Garage button ──
    void addGarageButton() {
        auto winSize = CCDirector::get()->getWinSize();
        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("The Garage", "goldFont.fnt", "GJ_button_05.png", 0.7f),
            this, menu_selector(MenuLayer::onGarageButton)
        );
        auto menu = CCMenu::create(btn, nullptr);
        menu->setPosition({winSize.width - 75, 50});
        this->addChild(menu, 10);
    }

    // Triggered: player clicks The Garage button
    void onGarageButton(CCObject*) {
        CCDirector::get()->replaceScene(
            CCTransitionFade::create(0.5f, GarageLayer::scene())
        );
    }

    // ── The Basement button ──
    void addBasementButton() {
        auto winSize = CCDirector::get()->getWinSize();
        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("The Basement", "goldFont.fnt", "GJ_button_05.png", 0.7f),
            this, menu_selector(MenuLayer::onBasementButton)
        );
        auto menu = CCMenu::create(btn, nullptr);
        menu->setPosition({winSize.width - 75, 95});
        this->addChild(menu, 10);
    }

    // SCENE: basement_gnomes
    // Triggered: player clicks The Basement button
    void onBasementButton(CCObject*) {
        auto popup = DialoguePopup::create(
            loadScene("basement_gnomes"),
            [this](std::string) {
                // After Jeff talks → send player to gnome training level
                GameManager::get()->setIntValue(KEY_BASEMENT_DONE, 1);
                GameManager::get()->setIntValue(KEY_IN_GNOME_LEVEL, 1);
                // Player goes back to menu and plays the gnome level
                CCDirector::get()->replaceScene(
                    CCTransitionFade::create(0.5f, MenuLayer::scene(false))
                );
            }
        );
        this->addChild(popup, 200);
    }
};

// ═══════════════════════════════════════════════════════════
//  PLAY LAYER HOOK
//  Detects when the player beats a specific level
// ═══════════════════════════════════════════════════════════
class $modify(PlayLayer) {
    void levelComplete() {
        PlayLayer::levelComplete();

        int levelID = m_level->m_levelID;
        auto* gm = GameManager::get();

        // Triggered: player beats the monster's challenge level (Act 1)
        if (levelID == CHALLENGE_LEVEL_ID
            && gm->getIntValue(KEY_IN_CHALLENGE)
            && !gm->getIntValue(KEY_CHALLENGE_DONE)) {
            gm->setIntValue(KEY_IN_CHALLENGE, 2); // MenuLayer picks this up
        }

        // Triggered: player beats Jeff's gnome training level (Act 2)
        if (levelID == GNOME_LEVEL_ID
            && gm->getIntValue(KEY_IN_GNOME_LEVEL)
            && !gm->getIntValue(KEY_GNOME_LEVEL_DONE)) {
            gm->setIntValue(KEY_IN_GNOME_LEVEL, 2); // MenuLayer picks this up
        }
    }
};

// ═══════════════════════════════════════════════════════════
//  PROFILE PAGE HOOK
//  Detects when the player visits GideonGleeful's profile
//  This triggers the account_enter_gideon scene
// ═══════════════════════════════════════════════════════════
#include <Geode/modify/ProfilePage.hpp>
class $modify(ProfilePage) {
    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) return false;

        // Check if this profile's username matches Gideon's account
        // We flag it and the MenuLayer picks it up when player goes back
        auto* gm = GameManager::get();
        bool fightPrepDone = gm->getIntValue(KEY_FIGHT_PREP_SHACK_DONE);
        bool alreadyTriggered = gm->getIntValue(KEY_ON_GIDEON_ACCOUNT);

        if (fightPrepDone && !alreadyTriggered) {
            // Get the username shown on this profile page
            // We use a short delay so the page has time to load the name
            this->scheduleOnce([this](float) {
                // m_username is the name shown on the profile
                // If it matches Gideon's account name, flag it
                if (m_username == GIDEON_ACCOUNT_NAME) {
                    GameManager::get()->setIntValue(KEY_ON_GIDEON_ACCOUNT, 1);
                    // Show a small hint popup right on the profile page
                    auto lines = loadScene("account_enter_gideon");
                    auto popup = DialoguePopup::create(lines);
                    this->addChild(popup, 200);
                }
            }, 1.0f);
        }

        return true;
    }
};
    }

    // Character Fallbacks
    auto* player = SimplePlayer::create(0);
    if (speaker == "Dipper") player->setColors(ccc3(50, 80, 200), ccc3(200, 210, 255));
    else if (speaker == "Mabel") player->setColors(ccc3(220, 60, 180), ccc3(255, 180, 240));
    else player->setColors(ccc3(150, 150, 150), ccc3(255, 255, 255));

    player->updatePlayerFrame(1, IconType::Cube);
    player->setScale(0.8f);
    return player;
}
//  TEXT HELPER
//  Replaces %username with the player's actual GD name
 
static std::string fillText(const std::string& text) {
    std::string username = GameManager::get()->m_playerName;
    std::string result = text;
    
    // Find %username and replace it with the real name
    std::string placeholder = "%username";
    size_t pos = result.find(placeholder);
    
    while (pos != std::string::npos) {
        result.replace(pos, placeholder.length(), username);
        pos = result.find(placeholder, pos + username.length());
    }
    
    return result;
}
 
//  DIALOGUE POPUP (With Visual Effects)
 
class DialoguePopup : public CCLayer {
public:
    std::vector<DialogueLine> m_lines;
    int m_index = 0;
    std::function<void(std::string)> m_onDone;
    CCLabelBMFont* m_textLabel;
    std::string m_fullTargetText;
    int m_charIndex = 0;

    static DialoguePopup* create(const std::vector<DialogueLine>& lines, std::function<void(std::string)> onDone = nullptr) {
        auto* ret = new DialoguePopup();
        if (ret && ret->init()) {
            ret->m_lines = lines;
            ret->m_onDone = onDone;
            ret->autorelease();
            ret->showLine(0);
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void showLine(int idx) {
        this->removeAllChildrenWithCleanup(true);
        if (idx >= m_lines.size()) {
            if (m_onDone) m_onDone("done");
            this->removeFromParent();
            return;
        }

        auto& line = m_lines[idx];
        auto winSize = CCDirector::get()->getWinSize();

        // 1. Background Fade
        auto bg = CCLayerColor::create({0,0,0,140});
        this->addChild(bg);

        // 2. Panel with "Bounce In" effect
        auto panel = CCScale9Sprite::create("GJ_square02.png");
        panel->setContentSize({360, 110});
        panel->setPosition({winSize.width / 2, winSize.height / 2 - 50});
        panel->setScale(0.5f);
        panel->runAction(CCEaseBackOut::create(CCScaleTo::create(0.4f, 1.0f)));
        this->addChild(panel);

        // 3. Speaker Portrait
        auto face = makeFace(line.face, line.speaker);
        face->setPosition({winSize.width/2 - 130, winSize.height/2 - 50});
        this->addChild(face);

        // 4. Name Label
        auto name = CCLabelBMFont::create(line.speaker.c_str(), "goldFont.fnt");
        name->setScale(0.6f);
        name->setAnchorPoint({0, 0.5f});
        name->setPosition({winSize.width/2 - 85, winSize.height/2 - 25});
        this->addChild(name);

        // 5. Typewriter Text Logic
        m_fullTargetText = fillText(line.text);
        m_charIndex = 0;
        m_textLabel = CCLabelBMFont::create("", "chatFont.fnt");
        m_textLabel->setScale(0.5f);
        m_textLabel->setWidth(220);
        m_textLabel->setAnchorPoint({0, 1.0f});
        m_textLabel->setPosition({winSize.width/2 - 85, winSize.height/2 - 40});
        this->addChild(m_textLabel);

        this->schedule(schedule_selector(DialoguePopup::updateTypewriter), 0.03f);

        // 6. Next Button (Delayed so they read the text)
        auto btnSpr = ButtonSprite::create(">>", "goldFont.fnt", "GJ_button_01.png", 0.6f);
        auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(DialoguePopup::onNext));
        auto menu = CCMenu::create(btn, nullptr);
        menu->setPosition({winSize.width/2 + 150, winSize.height/2 - 85});
        menu->setOpacity(0);
        menu->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCFadeIn::create(0.2f), nullptr));
        this->addChild(menu);
    }

    void updateTypewriter(float dt) {
        if (m_charIndex < m_fullTargetText.length()) {
            m_charIndex++;
            m_textLabel->setString(m_fullTargetText.substr(0, m_charIndex).c_str());
        } else {
            this->unschedule(schedule_selector(DialoguePopup::updateTypewriter));
        }
    }

    void onNext(CCObject*) {
        // If user clicks while text is still typing, finish the text immediately
        if (m_charIndex < m_fullTargetText.length()) {
            m_charIndex = m_fullTargetText.length();
            m_textLabel->setString(m_fullTargetText.c_str());
            this->unschedule(schedule_selector(DialoguePopup::updateTypewriter));
        } else {
            showLine(++m_index);
        }
    }
};