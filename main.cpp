#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

// ───────────────────────────────────────────────────────────
//  FACE SPRITE HELPER
// ───────────────────────────────────────────────────────────
static CCNode* makeFace(const std::string& faceName, const std::string& speaker) {
    std::string filename = faceName + ".png";
    auto* sprite = CCSprite::create(filename.c_str());
    
    if (sprite) {
        sprite->setScale(70.f / sprite->getContentSize().width);
        return sprite;
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
// ───────────────────────────────────────────────────────────
//  TEXT HELPER
//  Replaces %username with the player's actual GD name
// ───────────────────────────────────────────────────────────
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
// ───────────────────────────────────────────────────────────
//  DIALOGUE POPUP (With Visual Effects)
// ───────────────────────────────────────────────────────────
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