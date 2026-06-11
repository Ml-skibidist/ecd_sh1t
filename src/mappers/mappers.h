#ifndef MAPPERS_H
#define MAPPERS_H

#include <memory>

enum class MapperMode : unsigned char
{
	Enable,
	Disable,
};

#define MAPPER_MODE_ENABLE MapperMode::Enable
#define MAPPER_MODE_DISABLE MapperMode::Disable

class IMapper
{
public:
	IMapper() = default;
	virtual ~IMapper() = default;

	virtual bool Map(MapperMode map_mode, const char* detect) const = 0;
};

class UserDirMapper : public IMapper
{
public:
	UserDirMapper() = default;
	virtual ~UserDirMapper() override = default;

	virtual bool Map(MapperMode map_mode, const char* detect) const override;
};
class GameDirMapper : public IMapper
{
public:
	GameDirMapper() = default;
	virtual ~GameDirMapper() override = default;

	virtual bool Map(MapperMode map_mode, const char* detect) const override;
};
class PatternMapper : public IMapper
{
public:
	PatternMapper() = default;
	virtual ~PatternMapper() override = default;

	virtual bool Map(MapperMode map_mode, const char* detect) const override;
};

class BaseMapper
{
private:
	BaseMapper(BaseMapper&&) = delete;
	BaseMapper(const BaseMapper&) = delete;
	BaseMapper& operator=(BaseMapper&&) = delete;
	BaseMapper& operator=(const BaseMapper&) = delete;

	std::unique_ptr<IMapper> mapperPtr;

public:
	explicit BaseMapper(std::unique_ptr<IMapper> mapper_ptr) : mapperPtr(std::move(mapper_ptr)) {}
	~BaseMapper() = default;

	IMapper* GetMapper();
	void SetMapper(std::unique_ptr<IMapper> mapper_ptr);

	static BaseMapper& Get();
};

#define MAP_USERDIR_ENABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<UserDirMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_ENABLE, detect))
#define MAP_GAMEDIR_ENABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<GameDirMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_ENABLE, detect))
#define MAP_PATTERN_ENABLE(detect) \
	(BaseMapper::Get().SetMapper(std::make_unique<PatternMapper>()), \
	BaseMapper::Get().GetMapper()->Map(MAPPER_MODE_ENABLE, detect))
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