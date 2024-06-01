// #ifndef __COMMON_LOG_H__
// #define __COMMON_LOG_H__
// #include "Common/common_datetime.h"
// #include "Common/common_logengine.h"

// #ifndef __FILENAME__
// #define __FILENAME__ __FILE__
// #endif

// using GameJayo::Server::CLog;
// using GameJayo::Server::log_mask_debug;
// using GameJayo::Server::log_mask_info;
// using GameJayo::Server::log_mask_system_error;
// using GameJayo::Server::log_mask_system_warning;

// extern CLog *g_pLogger;

// int32_t InitLogger(const char *szFileName, int32_t iLogLevel, int32_t iLogID);

// int32_t InitLogger(CLog *pLogger);

// std::string Field(const char *prefix);

// template <typename Key, typename Value, typename... Args>
// std::string Field(const char *prefix, Key key, Value value, Args... args) {
//     std::ostringstream oss;
//     oss << prefix << key << ":" << value << Field(",", args...);
//     return oss.str();
// }

// // 调试日志：主要用于开发过程中的问题定位（线上默认：不打印）
// #define LOG_DEBUG(field, format, ...)              \
//     if (g_pLogger->WhetherDoLog(log_mask_debug)) { \
//         g_pLogger->Log(log_mask_debug,             \
//                        "%s:%d:%s|{%s}|" format,    \
//                        __FILENAME__,               \
//                        __LINE__,                   \
//                        __FUNCTION__,               \
//                        field,                      \
//                        ##__VA_ARGS__);             \
//     }

// // 信息日志：记录重要的操作或数据（线上默认：打印）
// #define LOG_INFO(field, format, ...)              \
//     if (g_pLogger->WhetherDoLog(log_mask_info)) { \
//         g_pLogger->Log(log_mask_info,             \
//                        "%s:%d:%s|{%s}|" format,   \
//                        __FILENAME__,              \
//                        __LINE__,                  \
//                        __FUNCTION__,              \
//                        field,                     \
//                        ##__VA_ARGS__);            \
//     }

// // 警告日志：需要记录的错误，但不一定需要立即解决（线上默认：打印）
// #define LOG_WARN(field, format, ...)                        \
//     if (g_pLogger->WhetherDoLog(log_mask_system_warning)) { \
//         g_pLogger->Log(log_mask_system_warning,             \
//                        "%s:%d:%s|{%s}|" format,             \
//                        __FILENAME__,                        \
//                        __LINE__,                            \
//                        __FUNCTION__,                        \
//                        field,                               \
//                        ##__VA_ARGS__);                      \
//     }

// // 错误日志：需要立即解决的错误（线上默认：打印 & 告警）
// #define LOG_ERROR(field, format, ...)                     \
//     if (g_pLogger->WhetherDoLog(log_mask_system_error)) { \
//         g_pLogger->Log(log_mask_system_error,             \
//                        "%s:%d:%s|{%s}|" format,           \
//                        __FILENAME__,                      \
//                        __LINE__,                          \
//                        __FUNCTION__,                      \
//                        field,                             \
//                        ##__VA_ARGS__);                    \
//     }

// #define SET_LOG_LEVEL(level) \
//     g_pLogger->SetLogLevel(level);

// // LOG FIELD
// #define FIELD(...)          Field("", ##__VA_ARGS__).c_str()
// #define FIELD_UIN(uin, ...) Field("", "uin", uin, ##__VA_ARGS__).c_str()
// #define FIELD_TID(...)      Field("", "thread", gettid(), ##__VA_ARGS__).c_str()
// #define FIELD_MATCHBATTLE(battleid, ...) Field("", "battleid", battleid, ##__VA_ARGS__).c_str()
// #define FIELD_GAMESEQUENCE(game_seq, ...) Field("", "game_seq", game_seq, ##__VA_ARGS__).c_str()
// #define FIELD_GAMESEQUENCE_BATTLE(game_seq, battleid, ...) Field("", "game_seq", game_seq, "battleid", battleid, ##__VA_ARGS__).c_str()
// #define FIELD_LEGION_UIN(legionid, lsid, uin, ...)  Field("", "legionid", legionid, "lsid", lsid, "uin", uin, ##__VA_ARGS__).c_str()
// #define FIELD_PLAYER_FLOW(player, ...)  Field("", "lsid", player->lsid(), "account", player->account().c_str(), "uin", player->uin(),  \
//         "name", player->name().c_str(), "legionid", player->legionId(), "legionname", player->legionName(),  ##__VA_ARGS__).c_str()

// #define FIELD_PLAYER FIELD("module", "player", "uin", player->uin(), "name", player->const_main_().rolename())
// #define FIELD_GAME FIELD("module", "game", "table", table_id_, "session", battle->session_, "battle_type", GetBattleTypeName())

// #define FIELD_BATTLE FIELD("module", "battle", "lsid", game_->lsid, "battle_type", BattleType_Name(GetBattleType()).c_str(), \
//     "table", game_->table_id_, "session", session_)

// // NOTE：额外补充了 gamer1, gamer2, legionid1, legionid2 可用于军团争霸战斗追踪
// #define FIELD_BATTLE_DETAIL FIELD("module", "battle", "lsid", game_->lsid, "battle_type", BattleType_Name(GetBattleType()).c_str(), \
//     "table", game_->table_id_, "session", session_, \
//     "gamer1", game_->getGamer1Uin(), "gamer2", game_->getGamer2Uin(), \
//     "legionid1", game_->getGamer1Legion(), "legionid2", game_->getGamer2Legion())

// // LOG CHECK
// #define CHECK_DEBUG(condition) \
//         do { if (!(condition)) { LOG_DEBUG("", "CHECK_DEBUG: %s", #condition); } \
//         } while(false)

// #define CHECK_INFO(condition) \
//         do { if (!(condition)) { LOG_INFO("", "CHECK_INFO: %s", #condition); } \
//         } while(false)

// #define CHECK_INFOS(condition, format, ...) \
//         do { if (!(condition)) { LOG_INFO("", "CHECK_INFOS: %s" format, #condition, ##__VA_ARGS__); } \
//         } while(false)

// #define CHECK_WARN(condition) \
//         do { if (!(condition)) { LOG_WARN("", "CHECK_WARN: %s", #condition); } \
//         } while(false)

// #define CHECK_ERROR(condition) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECK_ERROR: %s", #condition); } \
//         } while(false)

// #define CHECK_ERRORS(condition, format, ...) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECK_ERRORS: %s" format, #condition, ##__VA_ARGS__); } \
//         } while(false)

// // check void result
// #define CHECKVOID_DEBUG(condition) \
//         do { if (!(condition)) { LOG_DEBUG("", "CHECKVOID_DEBUG: %s", #condition); return; } \
//         } while(false)

// #define CHECKVOID_INFO(condition) \
//         do { if (!(condition)) { LOG_INFO("", "CHECKVOID_INFO: %s", #condition); return; } \
//         } while(false)
// #define CHECKVOID_INFOS(condition, format, ...) \
//         do { if (!(condition)) { LOG_INFO("", "CHECKVOID_INFOS: %s" format, #condition, ##__VA_ARGS__); return; } \
//         } while(false)

// #define CHECKVOID_WARN(condition) \
//         do { if (!(condition)) { LOG_WARN("", "CHECKVOID_WARN: %s", #condition); return; } \
//         } while(false)

// #define CHECKVOID_ERROR(condition) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECKVOID_ERROR: %s", #condition); return; } \
//         } while(false)

// #define CHECKVOID_ERRORS(condition, format, ...) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECKVOID_ERRORS: %s" format, #condition, ##__VA_ARGS__); return; } \
//         } while(false)

// // check bool result
// #define CHECKBOOL_DEBUG(condition) \
//         do { if (!(condition)) { LOG_DEBUG("", "CHECKBOOL_DEBUG: %s", #condition); return false; } \
//         } while(false)

// #define CHECKBOOL_INFO(condition) \
//         do { if (!(condition)) { LOG_INFO("", "CHECKBOOL_INFO: %s", #condition); return false; } \
//         } while(false)

// #define CHECKBOOL_INFOS(condition, format, ...) \
//         do { if (!(condition)) { LOG_INFO("", "CHECKBOOL_INFOS: %s" format, #condition, ##__VA_ARGS__); return false; } \
//         } while(false)

// #define CHECKBOOL_WARN(condition) \
//         do { if (!(condition)) { LOG_WARN("", "CHECKBOOL_WARN: %s", #condition); return false; } \
//         } while(false)

// #define CHECKBOOL_ERROR(condition) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECKBOOL_ERROR: %s", #condition); return false; } \
//         } while(false)

// #define CHECKBOOL_ERRORS(condition, format, ...) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECKBOOL_ERRORS: %s" format, #condition, ##__VA_ARGS__); return false; } \
//         } while(false)

// // check result
// #define CHECKRESULT_DEBUG(condition, result) \
//         do { if (!(condition)) { LOG_DEBUG("", "CHECKRESULT_DEBUG: %s", #condition); return result; } \
//         } while(false)

// #define CHECKRESULT_INFO(condition, result) \
//         do { if (!(condition)) { LOG_INFO("", "CHECKRESULT_INFO: %s", #condition); return result; } \
//         } while(false)

// #define CHECKRESULT_INFOS(condition, result, format, ...) \
//         do { if (!(condition)) { LOG_INFO("", "CHECKRESULT_INFOS: %s" format, #condition, ##__VA_ARGS__); return result; } \
//         } while(false)

// #define CHECKRESULT_WARN(condition, result) \
//         do { if (!(condition)) { LOG_WARN("", "CHECKRESULT_WARN: %s", #condition); return result; } \
//         } while(false)

// #define CHECKRESULT_ERROR(condition, result) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECKRESULT_ERROR: %s", #condition); return result; } \
//         } while(false)

// #define CHECKRESULT_ERRORS(condition, result, format, ...) \
//         do { if (!(condition)) { LOG_ERROR("", "CHECKRESULT_ERRORS: %s" format, #condition, ##__VA_ARGS__); return result; } \
//         } while(false)

// #endif  //__COMMON_LOG_H__
