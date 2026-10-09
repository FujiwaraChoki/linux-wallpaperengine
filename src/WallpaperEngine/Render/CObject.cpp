#include "CObject.h"

#include <utility>

using namespace WallpaperEngine;
using namespace WallpaperEngine::Render;
using namespace WallpaperEngine::Render::Wallpapers;

CObject::CObject (Wallpapers::CScene& scene, const Object& object) :
    Helpers::ContextAware (scene), m_scene (scene), m_object (object) { }

void CObject::setup () { }
void CObject::render () { }

Wallpapers::CScene& CObject::getScene () const { return this->m_scene; }

const AssetLocator& CObject::getAssetLocator () const { return this->getScene ().getAssetLocator (); }

int CObject::getId () const { return this->m_object.id; }

const Object& CObject::getObject () const { return this->m_object; }

bool CObject::hasHiddenAncestor () const {
    constexpr int kMaxParentDepth = 32;

    const Object* current = &this->m_object;

    for (int depth = 0; depth < kMaxParentDepth && current->parent.has_value (); depth++) {
	const auto* parent = this->m_scene.getObject (current->parent.value ());

	if (parent == nullptr) {
	    return false;
	}

	current = &parent->getObject ();

	const UserSetting* visible = nullptr;

	if (const auto* image = dynamic_cast<const Image*> (current)) {
	    visible = image->visible.get ();
	} else if (const auto* text = dynamic_cast<const Text*> (current)) {
	    visible = text->visible.get ();
	} else if (const auto* particle = dynamic_cast<const Particle*> (current)) {
	    visible = particle->visible.get ();
	}

	if (visible != nullptr && !visible->value->getBool ()) {
	    return true;
	}
    }

    return false;
}
