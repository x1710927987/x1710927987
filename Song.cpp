#include "Song.h"
#include <algorithm>
#include <iostream>
#include <cctype>

namespace {

// 去除首尾空白
std::string trim_copy(const std::string& s) {
    auto first = std::find_if_not(s.begin(), s.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    auto last = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();
    return (first < last) ? std::string(first, last) : std::string();
}

// 转小写
std::string to_lower_copy(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
        return std::tolower(ch);
    });
    return result;
}

// 拼接标签
std::string join_tags(const std::vector<std::string>& tags) {
    if (tags.empty()) return "";
    std::string result;
    for (size_t i = 0; i < tags.size() - 1; ++i) {
        result += tags[i] + ", ";
    }
    result += tags.back();
    return result;
}

} // namespace

int Song::next_id_ = 1;

// ---------------- 构造函数 ----------------
Song::Song(const std::string& title, const std::string& artist, int duration_sec, int rating) {
    id_ = 0;
    title_.clear();
    artist_.clear();
    duration_sec_ = 0;
    rating_ = 0;
    tags_.clear();
    valid_ = false;

    std::string trimmedTitle = trim_copy(title);
    std::string trimmedArtist = trim_copy(artist);

    if (trimmedTitle.empty()) {
        std::cout << "[错误] 标题不能为空\n";
        return;
    }
    if (trimmedArtist.empty()) {
        std::cout << "[错误] 艺人不能为空\n";
        return;
    }
    if (duration_sec <= 0) {
        std::cout << "[错误] 时长必须为正整数（秒）\n";
        return;
    }
    if (rating < 1 || rating > 5) {
        std::cout << "[错误] 评分必须在 1...5 之间\n";
        return;
    }

    id_ = next_id_++;
    title_ = trimmedTitle;
    artist_ = trimmedArtist;
    duration_sec_ = duration_sec;
    rating_ = rating;
    valid_ = true;
}

// ---------------- Setters ----------------
bool Song::set_title(const std::string& t) {
    std::string trimmed = trim_copy(t);
    if (trimmed.empty()) {
        std::cout << "[提示] 标题不能为空，已忽略本次修改\n";
        return false;
    }
    title_ = trimmed;
    return true;
}

bool Song::set_artist(const std::string& a) {
    std::string trimmed = trim_copy(a);
    if (trimmed.empty()) {
        std::cout << "[提示] 艺人不能为空，已忽略本次修改\n";
        return false;
    }
    artist_ = trimmed;
    return true;
}

bool Song::set_duration(int sec) {
    if (sec <= 0) {
        std::cout << "[提示] 时长需为正整数，已忽略本次修改\n";
        return false;
    }
    duration_sec_ = sec;
    return true;
}

bool Song::set_rating(int r) {
    if (r < 1 || r > 5) {
        std::cout << "[提示] 评分需在 1..5，已忽略本次修改\n";
        return false;
    }
    rating_ = r;
    return true;
}

// ---------------- 标签管理 ----------------
bool Song::add_tag(const std::string& tag) {
    std::string trimmedTag = trim_copy(tag);
    if (trimmedTag.empty()) {
        std::cout << "[提示] 空标签已忽略\n";
        return false;
    }

    std::string lowerTag = to_lower_copy(trimmedTag);
    for (const auto& existingTag : tags_) {
        if (to_lower_copy(existingTag) == lowerTag) {
            std::cout << "[提示] 标签已存在（忽略大小写）\n";
            return false;
        }
    }

    tags_.push_back(trimmedTag);
    return true;
}

bool Song::remove_tag(const std::string& tag) {
    std::string trimmedTag = trim_copy(tag);
    std::string lowerTag = to_lower_copy(trimmedTag);

    for (size_t i = 0; i < tags_.size(); ++i) {
        if (to_lower_copy(tags_[i]) == lowerTag) {
            tags_.erase(tags_.begin() + i);
            return true;
        }
    }

    std::cout << "[提示] 未找到该标签\n";
    return false;
}

// ---------------- 匹配函数 ----------------
bool Song::matches_keyword(const std::string& kw) const {
    std::string trimmedKw = trim_copy(kw);
    if (trimmedKw.empty()) return false;
    std::string lowerKw = to_lower_copy(trimmedKw);

    if (to_lower_copy(title_).find(lowerKw) != std::string::npos) return true;
    if (to_lower_copy(artist_).find(lowerKw) != std::string::npos) return true;

    for (const auto& tag : tags_) {
        if (to_lower_copy(tag).find(lowerKw) != std::string::npos) {
            return true;
        }
    }
    return false;
}

// ---------------- 输出重载 ----------------
std::ostream& operator<<(std::ostream& os, const Song& s) {
    os << "[#" << s.id_ << "] "
       << s.artist_ << " - "
       << s.title_ << " (" << s.duration_sec_ << "s) "
       << std::string(s.rating_, '*');

    if (!s.tags_.empty()) {
        os << "  [tags: " << join_tags(s.tags_) << "]";
    }
    return os;
}

// ---------------- 排序规则 ----------------
bool operator<(const Song& a, const Song& b) {
    // 1. 评分高的排前
    if (a.rating_ > b.rating_) return true;
    if (a.rating_ < b.rating_) return false;

    // 2. 标题字典序升序
    std::string aTitleLower = to_lower_copy(a.title_);
    std::string bTitleLower = to_lower_copy(b.title_);
    if (aTitleLower < bTitleLower) return true;
    if (aTitleLower > bTitleLower) return false;

    // 3. ID 小的排前
    return a.id_ < b.id_;
}