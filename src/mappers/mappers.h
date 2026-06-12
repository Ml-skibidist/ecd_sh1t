/* mappers.h - mapped methods */

#ifndef MAPPERS_H
#define MAPPERS_H

// PreCompiled includes
#include <memory>

// Methods for mappers
enum class MapperMode : unsigned char
{
	Enable,
	Disable,
};

// Defined methods
#define MAPPER_MODE_ENABLE MapperMode::Enable
#define MAPPER_MODE_DISABLE MapperMode::Disable

// Clear-virtual class for abstracted methods
class IMapper
{
public:
	IMapper() = default;
	virtual ~IMapper() = default;

	// Just overrideing this func in methods...
	virtual bool Map(MapperMode map_mode, const char* detect) const = 0;
};

// UserDir detectable method
class UserDirMapper : public IMapper
{
public:
	UserDirMapper() = default;
	virtual ~UserDirMapper() override = default;

	// Overrided method for userdir detects
	virtual bool Map(MapperMode map_mode, const char* detect) const override;
};
// GameDir detectable method
class GameDirMapper : public IMapper
{
public:
	GameDirMapper() = default;
	virtual ~GameDirMapper() override = default;

	// Overrided method for gamedir detects
	virtual bool Map(MapperMode map_mode, const char* detect) const override;
};
// Pattern detectable method
class PatternMapper : public IMapper
{
public:
	PatternMapper() = default;
	virtual ~PatternMapper() override = default;

	// Overrided method for pattern detects
	virtual bool Map(MapperMode map_mode, const char* detect) const override;
};

// Manager for mappers
class BaseMapper
{
private:
	// Block move/copy
	BaseMapper(BaseMapper&&) = delete;
	BaseMapper(const BaseMapper&) = delete;
	BaseMapper& operator=(BaseMapper&&) = delete;
	BaseMapper& operator=(const BaseMapper&) = delete;

	// It`s a strategy
	std::unique_ptr<IMapper> mapperPtr;

public:
	// Constructor/Destructor
	explicit BaseMapper(std::unique_ptr<IMapper> mapper_ptr) : mapperPtr(std::move(mapper_ptr)) {}
	~BaseMapper() = default;

	// Interactive methods
	IMapper* GetMapper();
	void SetMapper(std::unique_ptr<IMapper> mapper_ptr);

	// We have a one singleton class
	static BaseMapper& Get();
};

// Defined ENABLE methods
#define MAP_USERDIR_ENABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<UserDirMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_ENABLE, detect))
#define MAP_GAMEDIR_ENABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<GameDirMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_ENABLE, detect))
#define MAP_PATTERN_ENABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<PatternMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_ENABLE, detect))

// Defined DISABLE methods
#define MAP_USERDIR_DISABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<UserDirMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_DISABLE, detect))
#define MAP_GAMEDIR_DISABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<GameDirMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_DISABLE, detect))
#define MAP_PATTERN_DISABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<PatternMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_DISABLE, detect))

#endif // MAPPERS_H