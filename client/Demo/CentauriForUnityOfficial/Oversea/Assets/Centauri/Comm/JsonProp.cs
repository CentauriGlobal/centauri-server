using System;
using System.Reflection;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace CentauriPay
{
	[AttributeUsage(AttributeTargets.Field)]
	public class JsonProp: Attribute
	{
		public readonly string Name;

		public JsonProp (string name)
		{
			this.Name = name;
		}
	}

	[AttributeUsage(AttributeTargets.Field)]
	public class JsonListProp: Attribute{
		public readonly Type ElementType;

		public JsonListProp(Type elementType) {
			this.ElementType = elementType;
		}
	}

	public class JsonSerializable {

		public JsonSerializable(){
		}

		public JsonSerializable(object json){
			if (json == null)
				throw new ArgumentNullException("json cannot be null");
			Dictionary<string,object> dict = json as Dictionary<string,object>;
			if (dict == null)
				throw new ArgumentException ("we take Dictionary<string,object> only");
			foreach (FieldInfo field in JsonInfo.GetInfo(this)) {
				object[] props = field.GetCustomAttributes(typeof(JsonProp),true);
				if(props==null || props.Length<1) continue;
				object[] listProps = field.GetCustomAttributes(typeof(JsonListProp),true);
				JsonProp prop = props[0] as JsonProp;
				JsonListProp list = null;
				if(listProps != null && listProps.Length>0) list = (JsonListProp) listProps[0];
				if(prop == null) continue;
				try{
					object value = null;
					if(dict.TryGetValue(prop.Name,out value)){
						if(list!=null){
							IList iList = value as IList;
							if(iList==null) continue;// iList = (IList)Activator.CreateInstance(typeof(List<object>));
							object currentList = field.GetValue(this);
							IList castedList = null;
							if(currentList==null) castedList = (IList) Activator.CreateInstance(field.FieldType);
							if(castedList==null) continue;
							foreach(object item in iList){
								//object tmpJson = MiniJSON.Json.Deserialize((string)item);
								object tmpJson = item;
								//Debug.Log("type : " + tmpJson.GetType() + ", " + tmpJson.ToString());
								if(tmpJson != null && typeof(Dictionary<string,object>).IsAssignableFrom(tmpJson.GetType())){
									//Debug.Log("item : " + item);
									object convt = Activator.CreateInstance(list.ElementType,new System.Object[]{item});
									castedList.Add(convt);
								}else{
									castedList.Add(Convert.ChangeType(item,list.ElementType));
								}
							}
							field.SetValue(this,castedList);
						}else if(typeof(JsonSerializable).IsAssignableFrom(field.FieldType)){
							object convertedValue = JsonInfo.InstantiateType(field.FieldType,value);
							if(convertedValue==null)continue;
							field.SetValue(this,convertedValue);
						}else{
							field.SetValue(this,Convert.ChangeType(value,field.FieldType));
						}
					}
				}catch(FieldAccessException e){
                    ULog.Log(e.Message);
				}catch(TargetException e){
                    ULog.Log(e.Message);
				}catch(ArgumentException e){
                    ULog.Log(e.Message);
				}
			}
		}
		
		public JsonSerializable(string json): this(CentauriPay.Json.Deserialize (json)){}

		public Dictionary<string, object> JsonDict{
			get{
				Dictionary<string,object> dict = new Dictionary<string, object>();
				foreach(FieldInfo field in JsonInfo.GetInfo(this)){
					object[] props = field.GetCustomAttributes(typeof(JsonProp),true);
					if(props==null || props.Length<1) continue;
					object[] listProps = field.GetCustomAttributes(typeof(JsonListProp),true);
					JsonProp prop = props[0] as JsonProp;
					JsonListProp list = null;
					if(listProps != null && listProps.Length>0) list = (JsonListProp) listProps[0];
					if(prop == null) continue;
					object fieldValue = null;
					try{
						fieldValue = field.GetValue(this);
					}catch(TargetException e){
                        ULog.Log(e.Message);
					}catch(NotSupportedException e){
                        ULog.Log(e.Message);
					}catch(FieldAccessException e){
                        ULog.Log(e.Message);
					}catch(ArgumentException e){
                        ULog.Log(e.Message);
					}
					if(fieldValue==null)continue;
					if(list != null){
						IList listValue = fieldValue as IList;
						if(listValue == null) continue;
						dict[field.Name] = listValue;
					}else if(typeof(JsonSerializable).IsAssignableFrom(field.FieldType)){
						dict[field.Name] = (fieldValue as JsonSerializable).JsonDict;
					}else{
						dict[field.Name] = fieldValue;
					}
				}
				return dict;
			}
		}

		public Dictionary<string, object> UnityJsonDict{
			get{
				Dictionary<string,object> dict = new Dictionary<string, object>();
				foreach(FieldInfo field in JsonInfo.GetInfo(this)){
					object[] props = field.GetCustomAttributes(typeof(JsonProp),true);
					if(props==null || props.Length<1) continue;
					object[] listProps = field.GetCustomAttributes(typeof(JsonListProp),true);
					JsonProp prop = props[0] as JsonProp;
					JsonListProp list = null;
					if(listProps != null && listProps.Length>0) list = (JsonListProp) listProps[0];
					if(prop == null) continue;
					object fieldValue = null;
					try{
						fieldValue = field.GetValue(this);
					}catch(TargetException e){
                        ULog.Log(e.Message);
					}catch(NotSupportedException e){
                        ULog.Log(e.Message);
					}catch(FieldAccessException e){
                        ULog.Log(e.Message);
					}catch(ArgumentException e){
                        ULog.Log(e.Message);
					}
					if(fieldValue==null)continue;
					if(list != null){
						IList listValue = fieldValue as IList;
						if(listValue == null) continue;
						dict[prop.Name] = listValue;
					}else if(typeof(JsonSerializable).IsAssignableFrom(field.FieldType)){
						dict[prop.Name] = (fieldValue as JsonSerializable).UnityJsonDict;
					}else{
						dict[prop.Name] = fieldValue;
					}
				}
				return dict;
			}
		}

		public override string ToString ()
		{
			return CentauriPay.Json.Serialize (UnityJsonDict);
		}

		public string ToUnityString()
		{
			return CentauriPay.Json.Serialize(JsonDict);
		}
	}

	public class JsonInfo{
		static Dictionary<Type,LinkedList<FieldInfo>> jsonFields = new Dictionary<Type, LinkedList<FieldInfo>>();
		static Dictionary<Type,ConstructorInfo> constructors = new Dictionary<Type, ConstructorInfo> ();

		public static LinkedList<FieldInfo> GetInfo(object target){
			Type targetType = target.GetType();
			if(!typeof(JsonSerializable).IsAssignableFrom(targetType))return new LinkedList<FieldInfo>();
			if(jsonFields.ContainsKey(targetType)){
				return jsonFields[targetType];
			}
			LinkedList<FieldInfo> list = new LinkedList<FieldInfo> ();
			foreach (FieldInfo field in targetType.GetFields(BindingFlags.Public|BindingFlags.Instance)) {
				if(field.IsPublic){
					object[] attrs = field.GetCustomAttributes(typeof(JsonProp),true);
					if(attrs!=null && attrs.Length>=1) list.AddLast(new LinkedListNode<FieldInfo>(field));
				}
			}
			jsonFields [targetType] = list;
			return list;
		}

		public static object InstantiateType(Type type, object value){
			if (!typeof(JsonSerializable).IsAssignableFrom (type))
				return null;
			if (constructors.ContainsKey (type)) {
				return constructors[type].Invoke(new object[]{value});
			}
			ConstructorInfo cons = type.GetConstructor (new Type[]{typeof(object)});
			if (cons == null)
				return null;
			constructors [type] = cons;
			return cons.Invoke(new object[]{value});
		}
	}
}

