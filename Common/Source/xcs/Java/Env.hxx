/*
 * Copyright 2010-2021 Max Kellermann <max.kellermann@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "Exception.hxx"
#include "Ref.hxx"
#include "Class.hxx"

namespace Java {

template<typename... Args>
LocalObject
NewObjectRethrow(JNIEnv *env, jclass cls, jmethodID method, Args... args)
{
	LocalObject result{env, env->NewObject(cls, method, args...)};
	RethrowException(env);
	return result;
}

template<typename... Args>
LocalObject
CallStaticObjectMethodRethrow(JNIEnv *env, jclass cls, jmethodID method,
			      Args... args)
{
	LocalObject result{env,
		env->CallStaticObjectMethod(cls, method, args...)};
	RethrowException(env);
	return result;
}

template<typename... Args>
LocalObject
CallObjectMethodRethrow(JNIEnv *env, jobject obj, jmethodID method,
			Args... args)
{
	LocalObject result{env, env->CallObjectMethod(obj, method, args...)};
	RethrowException(env);
	return result;
}

inline
Class GetClassRethrow(JNIEnv *env, const char* class_name) {
    Java::Class cls{ env, class_name };
    Java::RethrowException(env); // rethrow ClassNotFoundException
    return cls;
}

} // namespace Java
