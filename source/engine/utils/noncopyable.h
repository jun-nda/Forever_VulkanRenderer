#pragma once

/// <summary>
/// 经典写法，拷贝需要父类及成员全部支持拷贝才行
/// </summary>
namespace engine
{
	class NonCopyable{
	protected:
		NonCopyable( ) = default;
		~NonCopyable( ) = default;
	private:
		NonCopyable(const NonCopyable&) = delete;
		const NonCopyable& operator=(const NonCopyable&) = delete;
	};
}